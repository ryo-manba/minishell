import * as MS from "./minishell";
import * as Parser from "./parser";
import * as EV from "./envvar";

export function expand_shell_param(state: Parser.ParserState) {
    let pipeline: MS.Pipeline | null = state.pipelinelist.pipeline;
    while (pipeline) {
        let clause: MS.Clause | null = pipeline.clause;
        while (clause) {
            let redir = clause.redirs;
            while (redir) {
                expand_token_shell_param(state, redir.operand_right);
                redir = redir.next;
            }
            let stree = clause.stree;
            while (stree) {
                expand_token_shell_param(state, stree);
                stree = stree.right;
            }
            clause = clause.next;
        }
        pipeline = pipeline.next;
    }
}

/**
 * 与えられたSTreeについて、変数展開を破壊的に行う。
 * expand shell params in given stree DESTRUCTIVE.
 */
function expand_token_shell_param(state: Parser.ParserState, stree: MS.STree) {
    /**
     * 現在位置
     */
    let i = 0;
    /**
     * 変数の開始位置
     */
    let var_start = 0;
    /**
     * 展開モード
     * - SCAN: パラメータらしきものをキャッチしていない状態。
     * - VAR: "$"を捕捉した状態
     * - BRACE: "$"の後にさらに"{"を捕捉した状態
     */
    let stat: "SCAN" | "VAR" | "BRACE" = "SCAN";
    let subtokens: MS.WordList | null = null;
    let subhead: MS.WordList | null = subtokens;
    let quoted: "none" | "single" | "double" = "none";
    while (i <= stree.token.length) {
        if (stat === "SCAN") {
            if (i < stree.token.length && stree.token[i] === "'" && quoted === "none") {
                quoted = "single";
            } else if (i < stree.token.length && stree.token[i] === "'" && quoted === "single") {
                quoted = "none";
            } else if (i < stree.token.length && stree.token[i] === "\"" && quoted === "none") {
                quoted = "double";
            } else if (i < stree.token.length && stree.token[i] === "\"" && quoted === "double") {
                quoted = "none";
            } else if (i === stree.token.length ||
                (quoted !== "single" && stree.token[i] === "$")) {
                // 愁嘆に達した or SCANモードで "$"を見つけた
                // -> 手前までをsubtokensにセット
                if (var_start < i) {
                    const subword: MS.WordList = {
                        word: stree.token.substring(var_start, i),
                        i: var_start,
                        next: null,
                        concluded: 1,
                        starting_chartype: "_",
                        right_delimiter: stree.token[i],
                        lex_type: "TOKEN",
                    };
                    if (!subhead) {
                        subtokens = subword;
                        subhead = subtokens;
                    } else {
                        subhead.next = subword;
                        subhead = subhead.next;
                    }
                }
                // -> var_startをセットしてVARモードに遷移
                var_start = i;
                stat = "VAR";
            }
        } else if (stat === "VAR") {
            // if (i === var_start + 1 && stree.token[i] === "{") {
            //     // VARモードで、"$"の右隣に"{"を見つけた
            //     // -> BRACEモードに遷移
            //     stat = "BRACE";
            // }
            if (i === stree.token.length || !MS.char_is_for_name(stree.token[i], i - var_start - 1)) {
                const name = stree.token.substring(var_start + 1, i);
                const val = EV.get_var(state.varmap, name);
                // console.log(i, name, val, state.varmap);
                const subword: MS.WordList = {
                    word: val || "",
                    i: var_start,
                    next: null,
                    concluded: 1,
                    starting_chartype: "_",
                    right_delimiter: stree.token[i],
                    lex_type: "TOKEN",
                };
                if (!subhead) {
                    subtokens = subword;
                    subhead = subtokens;
                } else {
                    subhead.next = subword;
                    subhead = subhead.next;
                }
                // -> var_startをセットしてSCANモードに遷移
                var_start = i;
                stat = "SCAN";
                continue;
            }
        // } else if (stat === "BRACE") {
        }
        i += 1;
    }
    let expanded = "";
    subhead = subtokens;
    console.log(stree);
    while (subhead) {
        expanded += subhead.word;
        console.log(i, subtokens, expanded);
        subhead = subhead.next;
    }
    stree.token = expanded;
}
