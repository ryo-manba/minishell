import * as MS from "./minishell";
import * as Parser from "./parser";
import * as EV from "./envvar";

type SubtokenId = "DQuoted" | "SQuoted" | "Variable" | "Naked" | "WS" | "NS" | "Divider";
type ExSubtokenId = SubtokenId | "BracedVariable";
type Subtoken = {
    token: string;
    token_id: SubtokenId;
    next: Subtoken | null;
};

export function expand_shell_param(state: Parser.ParserState) {
    let pipeline: MS.Pipeline | null = state.pipelinelist.pipeline;
    while (pipeline) {
        let clause: MS.Clause | null = pipeline.clause;
        while (clause) {
            let redir = clause.redirs;
            while (redir) {
                expand_token_flow(state, redir.operand_right);
                redir = redir.next;
            }
            let stree = clause.stree;
            while (stree) {
                expand_token_flow(state, stree);
                stree = stree.right;
            }
            clause = clause.next;
        }
        pipeline = pipeline.next;
    }
}

/**
 * 与えられたSTreeについて、各種展開展開を破壊的に行う。
 * expand shell params in given stree DESTRUCTIVE.
 */
function expand_token_flow(state: Parser.ParserState, stree: MS.STree) {
    let subtokens: Subtoken | null = expand_token_shell_param(state, stree);

    expand_token_split(state, subtokens);

    let expanded = "";
    let subhead: Subtoken | null = subtokens;
    subhead = subtokens;
    console.log(stree);
    while (subhead) {
        expanded += subhead.token;
        subhead = subhead.next;
    }
    stree.token = expanded;
}

function push_back_subtoken(token: string, token_id: SubtokenId, subhead: Subtoken | null) {
    const st: Subtoken = { token, token_id, next: null, };
    if (subhead) {
        subhead.next = st;
        return st;
    }
    return st;
}

/**
 * 与えられたSTreeについて、変数展開を破壊的に行う。
 * expand shell params in given stree DESTRUCTIVE.
 */
function expand_token_shell_param(state: Parser.ParserState, stree: MS.STree) {
    function inner_expansion(state: Parser.ParserState, token_str: string) {
        /**
         * 現在位置
         */
        let i = 0;
        /**
         * 変数の開始位置
         */
        let var_start = 0;
        let stat: ExSubtokenId = "Naked";
        let subtokens: Subtoken | null = null;
        let subhead: Subtoken | null = subtokens;
        let finished_expectedly = false;
        while (i <= token_str.length) {
            // 終了条件
            if (stat === "SQuoted" && i < token_str.length && token_str[i] === "'") {
                // SQuoted
                // (クオートされていない)'に遭遇すること。
                subhead = push_back_subtoken(token_str.substring(var_start, i), stat, subhead);
                subtokens = subtokens || subhead;
                i += 1;
            } else if (stat === "DQuoted" && i < token_str.length && token_str[i] === "\"") {
                // DQuoted
                // (クオートされていない)"に遭遇すること。
                // TODO: Parameter Expansion
                const quoted_text = token_str.substring(var_start, i);
                let subtree = inner_expansion(state, quoted_text);
                let sub_text = "";
                while (subtree) {
                    sub_text += subtree.token;
                    subtree = subtree.next;
                }
                subhead = push_back_subtoken(sub_text, stat, subhead);
                subtokens = subtokens || subhead;
                i += 1;
            } else if (stat === "Variable" && var_start === i && token_str[i] === "{") {
                // BracedVariableの**開始**
                // **Variable**状態で、`$`の次に(つまり`var_start === i`の時に)`{`に遭遇すること。
                var_start = i + 1;
                stat = "BracedVariable";
                i += 1;
                continue;
            } else if (stat === "BracedVariable" && token_str[i] === "}") {
                // BracedVariableの**終了**
                // }に遭遇すること。
                const name = token_str.substring(var_start, i);
                const val = EV.get_var(state.varmap, name);
                subhead = push_back_subtoken(val || "", "Variable", subhead);
                subtokens = subtokens || subhead;
                i += 1;
            } else if (stat === "Variable" && !MS.char_is_for_name(token_str[i], i - var_start - 1)) {
                // Variable
                // NAME適格文字ではない文字または文字列終端に遭遇すること。
                const name = token_str.substring(var_start, i);
                const val = EV.get_var(state.varmap, name);
                subhead = push_back_subtoken(val || "", stat, subhead);
                subtokens = subtokens || subhead;
            } else if (stat === "Naked" && !MS.char_is_for_naked(token_str[i])) {
                // Naked
                // クオートされていない'"$または文字列終端に遭遇すること。
                // 長さが0になる場合はpushしない
                if (var_start < i) {
                    subhead = push_back_subtoken(token_str.substring(var_start, i), stat, subhead);
                    subtokens = subtokens || subhead;
                }
            } else {
                // 何も終わらなかった
                i += 1;
                continue;
            }
            // 開始条件
            if (i === token_str.length) {
                finished_expectedly = true;
                break;
            }
            if (token_str[i] === "'") {
                // SQuoted
                // (クオートされていない)'に遭遇すること。
                var_start = i + 1;
                stat = "SQuoted"
            } else if (token_str[i] === "\"") {
                // DQuoted
                // (クオートされていない)"に遭遇すること。
                var_start = i + 1;
                stat = "DQuoted"
            } else if (token_str[i] === "$") {
                // Variable
                // (クオートされていない)$に遭遇すること。
                var_start = i + 1;
                stat = "Variable"
            } else {
                var_start = i;
                stat = "Naked"
            }
            i += 1;
        }
        if (!finished_expectedly) {
            console.warn("[!!] finished unexpectely");
        }
        return subtokens;
    }
    const token_str = stree.token;
    return inner_expansion(state, token_str);
}

export function expand_split(state: Parser.ParserState) {
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
 * 与えられたSTreeについて、word splittingを破壊的に行う。
 * split word in given stree DESTRUCTIVE.
 */
function expand_token_split(state: Parser.ParserState, stree: Subtoken | null) {
    let st: Subtoken | null = stree;
    let xt: Subtoken | null = null;
    let xht: Subtoken | null = null;
    const ifsv = EV.get_var(state.varmap, "IFS");
    // 0. そもそも実施する？
    if (typeof ifsv === "string" && ifsv.length === 0) {
        return;
    }
    const ifs = ifsv || ` \t\n`;
    const ifs_ws = ifs.split("").filter(c => ` \t\n\v\f\r`.includes(c)).join("");
    const ifs_ns = ifs.split("").filter(c => !` \t\n\v\f\r`.includes(c)).join("");
    console.log({ ifs, ifs_ws, ifs_ns });
    console.log("WS(0)", JSON.stringify(st, null, 2));
    // 1. Variableサブトークンを空白・非空白に分割
    while (st) {
        if (st.token_id !== "Variable") {
            if (xt) {
                xt.next = st;
            }
            xt = st;
            xht = xht || xt;
            st = st.next;
            continue;
        }
        let i = 0;
        while (i < st.token.length) {
            let v = i;
            if (ifs_ns.includes(st.token[i])) {
                // NSトークン
                i += 1;
                while (i < st.token.length && ifs_ws.includes(st.token[i])) {
                    i += 1;
                }
                const nst: Subtoken = {
                    token_id: "NS",
                    token: st.token.substring(v, i),
                    next: null,
                };
                if (xt) {
                    xt.next = nst;
                }
                xt = nst;
                xht = xht || xt;
            } else if (ifs_ws.includes(st.token[i])) {
                // WSトークン
                i += 1;
                while (i < st.token.length && ifs_ws.includes(st.token[i])) {
                    i += 1;
                }
                const nst: Subtoken = {
                    token_id: "WS",
                    token: st.token.substring(v, i),
                    next: null,
                };
                if (xt) {
                    xt.next = nst;
                }
                xt = nst;
                xht = xht || xt;
            } else {
                // 通常トークン
                i += 1;
                while (i < st.token.length && !ifs_ws.includes(st.token[i]) && !ifs_ns.includes(st.token[i])) {
                    i += 1;
                }
                const nst: Subtoken = {
                    token_id: "Variable",
                    token: st.token.substring(v, i),
                    next: null,
                };
                if (xt) {
                    xt.next = nst;
                }
                xt = nst;
                xht = xht || xt;
            }
        }
        st = st.next;
    }
    console.log("WS(1)", JSON.stringify(xht, null, 2));
    // 2. NS*WSNS* の並びでDividerを入れる
    let ns_eating_ws = false;
    st = xht;
    xt = null;
    xht = null;
    while (st) {
        if (st.token_id === "WS") {
            if (ns_eating_ws) {
                st = st.next;
                continue;
            }
            if (st.next && (st.next.token_id === "WS" || st.next.token_id === "NS")) {
                st = st.next;
                continue;
            }
        }
        if (st.token_id === "NS") {
            const nst: Subtoken = {
                token_id: "Divider",
                token: "",
                next: null,
            };
            if (xt) {
                xt.next = nst;
            }
            xt = nst;
            xht = xht || xt;
            ns_eating_ws = true;
            st = st.next;
            continue;
        }
        if (xt) {
            xt.next = st;
        }
        xt = st;
        xht = xht || xt;
        ns_eating_ws = false;
        st = st.next;
    }
    if (xt) {
        xt.next = null;
    }
    console.log("WS(2)", JSON.stringify(xht, null, 2));
    // 3. WS+の並びでDividerを入れる
    st = xht;
    xt = null;
    xht = null;
    let head_subtoken = true;
    while (st) {
        if (st.token_id === "WS") {
            if (!head_subtoken && st.next) {
                const nst: Subtoken = {
                    token_id: "Divider",
                    token: "",
                    next: null,
                };
                if (xt) {
                    xt.next = nst;
                }
                xt = nst;
                xht = xht || xt;
            }
            st = st.next;
            head_subtoken = false;
            continue;
        }
        if (xt) {
            xt.next = st;
        }
        xt = st;
        xht = xht || xt;
        st = st.next;
        head_subtoken = false;
    }
    if (xt) {
        xt.next = null;
    }
    console.log("WS(3)", JSON.stringify(xht, null, 2));
    return xht;
}