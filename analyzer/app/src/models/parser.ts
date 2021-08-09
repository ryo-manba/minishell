import * as MS from "./minishell";

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

function new_andorlist(): MS.ANDORList {
    return {
        pipeline: new_pipeline(),
        next: null,
        joint: null,
    };
}

function new_pipelinelist(): MS.PipelineList {
    return {
        andorlist: new_andorlist(),
    };
}

export type ParseCursor = {
    andorlist: MS.ANDORList;
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

    finished: boolean;
    parse_error: string | null;
    error_location_token: MS.WordList | null;
};

/**
 * パーサを初期化
 */
export function init_parser(word: MS.WordList): ParserState {
    const plist = new_pipelinelist();
    const cursor: ParseCursor = {
        andorlist: plist.andorlist,
        pipeline: plist.andorlist.pipeline,
        clause: plist.andorlist.pipeline.clause,
        redir: null,
        stree: null,
        lexer_token: word,
    };
    return {
        pipelinelist: plist,
        cursor,
        word,
        finished: false,
        parse_error: null,
        error_location_token: null,
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

/**
 * パース本体
 */
export function parse_unit(state: ParserState) {
    const lexer_token = shift_lexer_token(state);
    if (!lexer_token) { return "exhausted"; }

    if (lexer_token.lex_type === "NEWLINE") {
        // [トークンがNEWLINEだった場合]
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
        };
        if (lexer_token.lex_type === "IO_NUMBER") {
            const next_token = shift_lexer_token(state);
            if (!next_token) {
                // シンタックスエラー
                state.parse_error = "SOLE_IO_NUMBER";
                state.error_location_token = lexer_token;
                return "error";
            }
            return subparse_redirection(state, next_token, st);
        }
        add_stree(state, st);
        console.log("parsed", lexer_token, st);
        return "continue";
    }
    if (lexer_token.lex_type == "OPERATOR") {
        // [トークンがなんらかの演算子だった場合]

        if (MS.ANDORListTerminateOperators.includes(lexer_token.word as any)) {
            // [ANDOR-Listを区切る演算子だった場合]
            // ANDORリスト終端処理を行う。
            return subparse_term_andor_list(state, lexer_token);
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
    state.parse_error = "NOT_CAPTURED";
    state.error_location_token = lexer_token;
    return "error";
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
 * ANDORリスト終端処理を行う。
 */
 function subparse_term_andor_list(
    state: ParserState,
    lexer_token: MS.WordList,
) {
    const serr = syntax_check_term_andor_list(state, false);
    if (serr) {
        state.parse_error = serr;
        state.error_location_token = lexer_token;
        return "error";
    }
    state.cursor.andorlist.joint = lexer_token.word as any;
    state.cursor.andorlist.next = new_andorlist();
    state.cursor.andorlist = state.cursor.andorlist.next;
    state.cursor.pipeline = state.cursor.andorlist.pipeline;
    state.cursor.clause = state.cursor.pipeline.clause;
    state.cursor.stree = null;
    state.cursor.redir = null;
    return "continue";
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
        state.parse_error = serr;
        state.error_location_token = lexer_token;
        return "error";
    }
    state.cursor.pipeline.joint = lexer_token.word as any;
    console.log(state.cursor.pipeline.joint);
    state.cursor.pipeline.next = new_pipeline();
    state.cursor.pipeline = state.cursor.pipeline.next;
    state.cursor.clause = state.cursor.pipeline.clause;
    state.cursor.stree = null;
    state.cursor.redir = null;
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
        state.parse_error = serr;
        state.error_location_token = lexer_token;
        return "error";
    }
    state.cursor.clause.next = new_clause();
    state.cursor.clause = state.cursor.clause.next;
    state.cursor.stree = null;
    state.cursor.redir = null;
    return "continue";
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
        state.parse_error = "NO_RIGHT_OPERAND";
        state.error_location_token = next_token || lexer_token;
        return "error";
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

type FlatANDOR = {
    andor: MS.ANDORList;
    pipelines: FlatPipeline[];
}

type FlatPipelineList = {
    pipelinelist: MS.PipelineList;
    andors: FlatANDOR[];
}

/**
 * パースして得られた再帰構造を非再帰化(配列化)する。
 * ※ minishell本編では無用。あくまで表示のためにやってる。
 */
export function flatten_pipelinelist(pipelinelist: MS.PipelineList) {
    const fpl: FlatPipelineList = {
        pipelinelist,
        andors: [],
    };
    let cursor_andor: MS.ANDORList | null = pipelinelist.andorlist;
    while (cursor_andor) {
        const flat_ao: FlatANDOR = {
            andor: cursor_andor,
            pipelines: [],
        }
        let cursor_pipeline: MS.Pipeline | null = cursor_andor.pipeline;
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
            flat_ao.pipelines.push(flat_pl);
            cursor_pipeline = cursor_pipeline.next;
        }
        fpl.andors.push(flat_ao)
        cursor_andor = cursor_andor.next;
    }
    return fpl;
}
