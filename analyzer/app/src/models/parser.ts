import * as MS from "./minishell";
import * as EV from "./envvar";

function new_clause(): MS.Clause {
    return {
        redirs: null,
        stree: null,
        next: null,
    };
}

function new_pipeline(): MS.Pipeline {
    return {
        clause: new_clause(),
        next: null,
        joint: null,
    };
}

function new_pipelinelist(): MS.PipelineList {
    return {
        pipeline: new_pipeline(),
    };
}

export type ParseCursor = {
    pipeline: MS.Pipeline;
    clause: MS.Clause;
    redir: MS.RedirList | null;
    stree: MS.STree | null;

    lexer_token: MS.WordList | null;
};

export type ParserState = {
    pipelinelist: MS.PipelineList;
    /**
     * パースの「現在位置」
     */
    cursor: ParseCursor;
    word: MS.WordList;
    for_subshell: boolean;

    finished: boolean;
    parse_error: string | null;
    error_location_token: MS.WordList | null;
    varmap: EV.ShellVarMap;
};

/**
 * パーサを初期化
 */
export function init_parser(
    word: MS.WordList,
    varmap: EV.ShellVarMap,
    for_subshell: boolean,
): ParserState {
    const plist = new_pipelinelist();
    const cursor: ParseCursor = {
        pipeline: plist.pipeline,
        clause: plist.pipeline.clause,
        redir: null,
        stree: null,
        lexer_token: word,
    };
    return {
        pipelinelist: plist,
        cursor,
        word,
        for_subshell,
        finished: false,
        parse_error: null,
        error_location_token: null,
        varmap,
    };
}

/**
 * cursor lexer_token の先頭を除去して返す。
 */
function shift_lexer_token(state: ParserState) {
    const token = state.cursor.lexer_token;
    if (!token) { return null; }
    state.cursor.lexer_token = token.next;
    return token;
}

/**
 * cursor stree(の右)に要素を追加する。
 * cursor streeがnullの場合もしかるべく処置する。
 */
function add_stree(state: ParserState, stree: MS.STree) {
    const cursor = state.cursor;
    if (cursor.stree) {
        cursor.stree.right = stree;
    } else {
        cursor.clause.stree = stree;
    }
    cursor.stree = stree;
}

/**
 * cursor redirに要素を追加する。
 * cursor redirがnullの場合もしかるべく処置する。
 */
 function add_redir(state: ParserState, redir: MS.RedirList) {
    const cursor = state.cursor;
    if (cursor.redir) {
        cursor.redir.next = redir;
    } else {
        cursor.clause.redirs = redir;
    }
    cursor.redir = redir;
}

function syntax_check_finalize(state: ParserState) {

}

function syntax_check_term_andor_list(state: ParserState, by_newline: boolean) {
    const subresult = syntax_check_term_pipeline(state, by_newline);
    if (subresult) {
        return subresult;
    }
    return null;
}

function syntax_check_term_pipeline(state: ParserState, by_newline: boolean) {
    const subresult = syntax_check_term_clause(state, by_newline);
    if (subresult) {
        return subresult;
    }
    return null;
}

function syntax_check_term_clause(state: ParserState, by_newline: boolean) {
    if (!state.cursor.clause.stree) {
        // streeが存在しない
        if (!by_newline) {
            // NEWLINEによる呼び出しでないならシンタックスエラー
            return "BLANK_CLAUSE";
        }
    }
    return null;
}

function str_is_for_name(str: string) {
    return !!str.match(/^[_A-Za-z][0-9_A-Za-z]*$/);
}

function is_assignment_word(state: ParserState, st: MS.STree) {
    if (st.token_id !== "WORD") { return false; }
    if (!state.cursor.stree) {
        // トークンが(clauseの)先頭である場合
        // トークンが=を含まないなら、ルール1(WORDか予約語か)に移行。
    }
    // トークンが=を含まないなら、このルールの評価を終了(たぶん)。
    const ieq = st.token.indexOf("=");
    if (ieq <= 0) { return false; }
    // =より前の部分がNAMEとして適格であれば、このトークンをASSIGNMENT_WORDとする。
    if (str_is_for_name(st.token.substring(0, ieq))) {
        return true;
    }
    return false;
}

function return_with_error(state: ParserState, lexer_token: MS.WordList, error_type: string) {
    state.parse_error = error_type;
    state.error_location_token = lexer_token;
    return "error";
}

/**
 * パース本体
 */
export function parse_unit(state: ParserState) {
    const lexer_token = shift_lexer_token(state);
    if (!lexer_token) { return "exhausted"; }

    if (lexer_token.lex_type === "NEWLINE") {
        // [トークンがNEWLINEだった場合]

        // サブシェル内だった場合はエラー
        if (state.for_subshell) {
            return return_with_error(state, lexer_token, "NEWLINE_IN_SUBSHELL");
        }

        // パース終了処理を行う。
        state.finished = true;
        // エラーチェック
        return "finish";
    }

    if (lexer_token.lex_type == "TOKEN" || lexer_token.lex_type == "IO_NUMBER") {
        // [トークンがTOKENまたはIO_NUMBERだった場合]

        // とりあえずトークンを取り込む
        const st: MS.STree = {
            token: lexer_token.word,
            token_id: lexer_token.lex_type == "TOKEN" ? "WORD" : "IO_NUMBER",
            depth: 1,
            left: null,
            right: null,
            subshell: null,
        };
        if (lexer_token.lex_type === "IO_NUMBER") {
            // [トークンがIO_NUMBERだった場合]
            // リダイレクションの記述として処理を進める
            const next_token = shift_lexer_token(state);
            if (!next_token) {
                // シンタックスエラー
                return return_with_error(state, lexer_token, "SOLE_IO_NUMBER");
            }
            return subparse_redirection(state, next_token, st);
        }

        // [先行するトークンがサブシェルだった場合]
        // -> エラー
        if (state.cursor.stree && state.cursor.stree.token_id ===  "SUBSHELL") {
            return return_with_error(state, lexer_token, "NEXT_TO_SUBSHELL");
        }

        // [トークンがTOKENだった場合]
        // トークンがASSINMENT_WORDであるかどうかをチェックする。
        if (is_assignment_word(state, st)) {
            st.token_id = "ASSIGNMENT_WORD";
        }
        add_stree(state, st);
        // console.log("parsed", lexer_token, st);
        return "continue";
    }
    if (lexer_token.lex_type == "OPERATOR") {
        // [トークンがなんらかの演算子だった場合]

        if (MS.SubshellOpenOperators.includes(lexer_token.word as any)) {
            // [サブシェルを開始する演算子だった場合]
            // ここから新たなサブシェルパースを開始する
            // ParseStateはまっさらな状態で開始するが、 state.cursor.lexer_token だけは引き継がれる。
            return parse_subshell(state, lexer_token);
        }

        if (MS.SubshellCloseOperators.includes(lexer_token.word as any)) {
            // [サブシェルを終了する演算子だった場合]
            // パースを終了する
            if (!state.for_subshell) {
                // サブシェル解析中でないのにサブシェル終了演算子に遭遇した場合はエラー
                return return_with_error(state, lexer_token, "UNEXPECTED_SUBSHELL_CLOSER");
            }
            // TODO: NEWLINEによる終了とは異なり、state.cursor.lexer_token が残っていてもOKとすることに注意。
            state.finished = true;
            // エラーチェック
            return "finish";
        }

        if (MS.PipelineTerminateOperators.includes(lexer_token.word as any)) {
            // [Pipelineを区切る演算子だった場合]
            // パイプライン終端処理を行う。
            return subparse_term_pipeline(state, lexer_token);
        }

        if (MS.ClauseTerminateOperators.includes(lexer_token.word as any)) {
            // [Clauseを区切る演算子だった場合]
            // 節終端処理を行う。
            return subparse_term_clause(state, lexer_token);
        }

        if (MS.RedirectionOperators.includes(lexer_token.word as any)) {
            // [リダイレクション演算子だった場合]
            return subparse_redirection(state, lexer_token, null);
        }

    }
    return return_with_error(state, lexer_token, "NOT_CAPTURED");
}

/**
 * サブシェルのパースを行う
 * (正確にはパースをキックする)
 */
function parse_subshell(state: ParserState, lexer_token: MS.WordList) {
    if (!state.cursor.lexer_token) {
        // lexerトークンが尽きていればエラー
        return return_with_error(state, lexer_token, "NO_RIGHT_ELEM");
    }
    if (state.cursor.stree) {
        // clauseの先頭でなければエラー
        return return_with_error(state, lexer_token, "SUBSHELL_IS_NOT_LEADING");
    }
    if (state.cursor.redir) {
        // リダイレクションがすでに記述されている場合はエラー
        return return_with_error(state, lexer_token, "REDIR_BEFORE_SUBSHELL");
    }
    const substate = init_parser(state.cursor.lexer_token, state.varmap, true);
    parse(substate);
    // パース完了後、結果(substate)からサブシェルを取り出す
    if (substate.parse_error && substate.error_location_token) {
        return return_with_error(state, substate.error_location_token, substate.parse_error);
    }
    state.cursor.lexer_token = substate.cursor.lexer_token;
    const subshell = substate.pipelinelist;
    const st: MS.STree = {
        token: "(subshell)",
        token_id: "SUBSHELL",
        depth: 1,
        left: null,
        right: null,
        subshell,
    };
    add_stree(state, st);
    return "continue";
}

export function parse(state: ParserState) {
    for(let n = 0; n < 1000; ++n) {
        if (state.finished) { break; }
        if (state.parse_error) { break; }
        const result = parse_unit(state);
        if (result === "exhausted") { break; }
    }
    return state;
}

/**
 * パイプライン終端処理を行う。
 */
function subparse_term_pipeline(
    state: ParserState,
    lexer_token: MS.WordList,
) {
    const serr = syntax_check_term_pipeline(state, false);
    if (serr) {
        return return_with_error(state, lexer_token, serr);
    }
    state.cursor.pipeline.joint = lexer_token.word as any;
    state.cursor.pipeline.next = new_pipeline();
    state.cursor.pipeline = state.cursor.pipeline.next;
    reset_cursor_pipeline(state);
    return "continue";
}

/**
 * 節終端処理を行う。
 */
function subparse_term_clause(
    state: ParserState,
    lexer_token: MS.WordList,
) {
    const serr = syntax_check_term_clause(state, false);
    if (serr) {
        return return_with_error(state, lexer_token, serr);
    }
    state.cursor.clause.next = new_clause();
    state.cursor.clause = state.cursor.clause.next;
    reset_cursor_clause(state);
    return "continue";
}

function reset_cursor_pipeline(state: ParserState) {
    state.cursor.clause = state.cursor.pipeline.clause;
    reset_cursor_clause(state);
}

function reset_cursor_clause(state: ParserState) {
    state.cursor.stree = null;
    state.cursor.redir = null;
}

/**
 * リダイレクションのパースを行う
 */
function subparse_redirection(
    state: ParserState,
    lexer_token: MS.WordList,
    /**
     * 先行するIO_NUMBERトークン(あれば)
     */
    ion_token: MS.STree | null,
) {
    // リダイレクション演算子トークンに続くlexerトークンがTOKENかどうか?
    const next_token = shift_lexer_token(state);
    if (!next_token || next_token.lex_type !== "TOKEN") {
        // 続くlexerトークンが存在しないか、トークン種別がTOKENではない
        // -> シンタックスエラー
        return return_with_error(state, next_token || lexer_token, "NO_RIGHT_OPERAND");
    }
    const redir: MS.RedirList = {
        next: null,
        operand_left: ion_token,
        operand_right: next_token,
        op: lexer_token.word as any,
    };
    add_redir(state, redir);
    return "continue";
}

type FlatClause = {
    clause: MS.Clause;
    redirs: MS.RedirList[];
    strees: MS.STree[];
};

type FlatPipeline = {
    pipeline: MS.Pipeline;
    clauses: FlatClause[];
}

export type FlatPipelineList = {
    pipelinelist: MS.PipelineList;
    pipelines: FlatPipeline[];
}

export type FlattenedState = ParserState | {
    flattened_pipelinelist: FlatPipelineList;
};

/**
 * パースして得られた再帰構造を非再帰化(配列化)する。
 * ※ minishell本編では無用。あくまで表示のためにやってる。
 */
export function flatten_pipelinelist(pipelinelist: MS.PipelineList) {
    const fpl: FlatPipelineList = {
        pipelinelist,
        pipelines: [],
    };
    let cursor_pipeline: MS.Pipeline | null = pipelinelist.pipeline;
    while (cursor_pipeline) {
        const flat_pl: FlatPipeline = {
            pipeline: cursor_pipeline,
            clauses: [],
        };
        let cursor_clause: MS.Clause | null = cursor_pipeline.clause;
        while (cursor_clause) {
            const flat_cl: FlatClause = {
                clause: cursor_clause,
                redirs: [],
                strees: [],
            };
            let cursor_redir: MS.RedirList | null = cursor_clause.redirs;
            while (cursor_redir) {
                flat_cl.redirs.push(cursor_redir);
                cursor_redir = cursor_redir.next;
            }
            let cursor_stree: MS.STree | null = cursor_clause.stree;
            while (cursor_stree) {
                flat_cl.strees.push(cursor_stree);
                cursor_stree = cursor_stree.right;
            }
            flat_pl.clauses.push(flat_cl);
            cursor_clause = cursor_clause.next;
        }
        fpl.pipelines.push(flat_pl);
        cursor_pipeline = cursor_pipeline.next;
    }
    return fpl;
}
