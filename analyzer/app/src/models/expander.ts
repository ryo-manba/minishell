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


function push_back_stree(t1: MS.STree, t2: MS.STree | null) {
    let st: MS.STree | null = t1;
    if (!st) { return null; }
    while (st && st.right) {
        st = st.right
    }
    if (st) {
        st.right = t2;
    }
    return st;
}

export function expand_shell_param(state: Parser.ParserState, pipelinelist: MS.PipelineList) {
    state.finished = false;
    let pipeline: MS.Pipeline | null = pipelinelist.pipeline;
    while (pipeline) {
        let clause: MS.Clause | null = pipeline.clause;
        while (clause) {
            let redir = clause.redirs;
            while (redir) {
                const original_token = redir.operand_right.token;
                const expanded = expand_token_flow(state, redir.operand_right);
                if (state.parse_error) {
                    return;
                }
                
                console.log(redir.operand_right, expanded);
                if (!expanded || expanded && expanded.right) {
                    state.parse_error = `${original_token}: ambiguous redirect`;
                    state.error_location_token = null;
                    console.log(state.parse_error)
                    return;
                }
                if (expanded) {
                    // TODO: !expanded の時エラー(ambiguous redirect)
                    // TODO: expanded && expanded.right の時エラー(ambiguous redirect)
                    redir.operand_right = expanded;
                }
                redir = redir.next;
            }
            let stree = clause.stree;
            let par_stree: MS.STree | null = null;
            while (stree) {
                const old_right = stree.right;
                const expanded = expand_token_flow(state, stree);
                if (expanded) {
                    if (par_stree) {
                        par_stree.right = expanded;
                    } else {
                        clause.stree = expanded;
                    }
                    par_stree = push_back_stree(expanded, old_right);
                } else {
                    if (par_stree) {
                        par_stree.right = old_right;
                    } else {
                        clause.stree = old_right;
                    }
                }
                stree = old_right;
            }
            clause = clause.next;
        }
        pipeline = pipeline.next;
    }
    state.finished = true;
    return pipelinelist;
}

/**
 * 与えられたSTreeについて、各種展開展開を破壊的に行う。
 * expand shell params in given stree DESTRUCTIVE.
 */
function expand_token_flow(state: Parser.ParserState, stree: MS.STree) {
    if (stree.subshell) {
        // サブシェルを再帰的に展開
        expand_shell_param(state, stree.subshell);
        if (!state.finished) {
            return stree;
        }
        return stree;
    }
    // 変数展開
    let subtokens: Subtoken | null = expand_token_shell_param(state, stree);
    // console.log(JSON.stringify(subtokens, null, 2));
    // ワードスプリット
    subtokens = expand_token_split(state, subtokens);
    // console.log(JSON.stringify(subtokens, null, 2));
    // ファイル名展開
    subtokens = expand_token_filename(state, subtokens);
    // console.log(JSON.stringify(subtokens, null, 2));
    // 連結
    const joined_stree = expand_token_join(state, subtokens);
    // console.log(JSON.stringify(joined_stree, null, 2));
    return joined_stree;
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
    function inner_expansion(
        state: Parser.ParserState,
        token_str: string,
        exquoted = false
    ) {
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
                let subtree = inner_expansion(state, quoted_text, true);
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
            if (!exquoted && token_str[i] === "'") {
                // SQuoted
                // (クオートされていない)'に遭遇すること。
                var_start = i + 1;
                stat = "SQuoted"
            } else if (!exquoted && token_str[i] === "\"") {
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
 * 与えられたSubtokenについて、word splittingを破壊的に行う。
 * split word in given stree DESTRUCTIVE.
 */
function expand_token_split(state: Parser.ParserState, stree: Subtoken | null) {
    let st: Subtoken | null = stree;
    let xt: Subtoken | null = null;
    let xht: Subtoken | null = null;
    const ifsv = EV.get_var(state.varmap, "IFS");
    // 0. そもそも実施する？
    if (typeof ifsv === "string" && ifsv.length === 0) {
        return stree;
    }
    const ifs = ` \t\n`; // ifsv
    const ifs_ws = ifs.split("").filter(c => ` \t\n\v\f\r`.includes(c)).join("");
    const ifs_ns = ifs.split("").filter(c => !` \t\n\v\f\r`.includes(c)).join("");
    // console.log({ ifs, ifs_ws, ifs_ns });
    // console.log("WS(0)", JSON.stringify(st, null, 2));
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
    // console.log("WS(1)", JSON.stringify(xht, null, 2));
    // 2. NS*WSNS* の並びでDividerを入れる
    // let ns_eating_ws = false;
    // st = xht;
    // xt = null;
    // xht = null;
    // while (st) {
    //     if (st.token_id === "WS") {
    //         if (ns_eating_ws) {
    //             st = st.next;
    //             continue;
    //         }
    //         if (st.next && (st.next.token_id === "WS" || st.next.token_id === "NS")) {
    //             st = st.next;
    //             continue;
    //         }
    //     }
    //     if (st.token_id === "NS") {
    //         const nst: Subtoken = {
    //             token_id: "Divider",
    //             token: "",
    //             next: null,
    //         };
    //         if (xt) {
    //             xt.next = nst;
    //         }
    //         xt = nst;
    //         xht = xht || xt;
    //         ns_eating_ws = true;
    //         st = st.next;
    //         continue;
    //     }
    //     if (xt) {
    //         xt.next = st;
    //     }
    //     xt = st;
    //     xht = xht || xt;
    //     ns_eating_ws = false;
    //     st = st.next;
    // }
    // if (xt) {
    //     xt.next = null;
    // }
    // console.log("WS(2)", JSON.stringify(xht, null, 2));
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
    // console.log("WS(3)", JSON.stringify(xht, null, 2));
    return xht;
}

/**
 * splitされたトークンをjoinする
 */
function expand_token_join(state: Parser.ParserState, stree: Subtoken | null) {
    let joined: MS.STree | null = null;
    let joined_head: MS.STree | null = null;
    let sublist: Subtoken | null = null;
    let subhead: Subtoken | null = null;
    while (true) {
        if (!stree || stree.token_id === "Divider") {
            if (sublist || !stree) {
                let subtoken: string | null = null;
                if (subhead) {
                    subhead.next = null;
                }
                // console.log("join", JSON.stringify(sublist, null, 2));
                while (sublist) {
                    subtoken = subtoken || "";
                    subtoken += sublist.token;
                    sublist = sublist.next;
                }
                if (typeof subtoken === "string") {
                    const st: MS.STree = {
                        token: subtoken,
                        token_id: "WORD",
                        left: null,
                        right: null,
                        subshell: null,
                    };
                    if (joined_head) {
                        joined_head.right = st;
                    }
                    joined_head = st;
                    joined = joined || joined_head;
                    sublist = null;
                    subhead = null;
                }
            }
        } else {
            if (subhead) {
                subhead.next = stree;
            }
            subhead = stree;
            sublist = sublist || subhead;
        }
        if (!stree) { break; }
        stree = stree.next;
        // console.log("stree", stree);
    }
    // console.log("joined", joined);
    return joined;
}

/**
 * 与えられたSubtokenについて、ファイル名展開を破壊的に行う。
 * expand file-names in given stree DESTRUCTIVE.
 */
function expand_token_filename(state: Parser.ParserState, stree: Subtoken | null) {
    // そもそも、展開する必要があるのか？

    // Dividerまたは末尾がきたら、それまでのサブトークンを判定に回す
    let retlist: Subtoken[] = [];
    let sublist: Subtoken[] = [];
    let subhead: Subtoken | null = stree;
    let files: string[] | null = null;
    let subnext: Subtoken | null = null;
    while (true) {
        if (subhead) {
            sublist.push(subhead);
        }
        subnext = subhead ? subhead.next : null;
        if ((!subhead && sublist.length > 0) ||
            (subhead && subhead.token_id === "Divider")) {
            // 判定
            // sublistのクオートされていないトークンに"*"が1つでも含まれていれば、展開を行う。
            if (subhead) {
                subhead.next = null;
            }
            const contains_star = sublist
                .filter(st => st.token_id !== "DQuoted" && st.token_id !== "SQuoted")
                .some(st => st.token.includes("*"));
            console.log(
                "FN",
                sublist
                    .filter(st => st.token_id !== "DQuoted" && st.token_id !== "SQuoted")
                    .map(st => st.token),
                contains_star,
                sublist,
            );
            if (contains_star && sublist.length > 0) {
                // ファイル名展開
                files = files || fetch_files(state);
                console.log({ files });
                let i = 0;
                while (i < files.length) {
                    const filename = files[i];
                    if (match_to_list(filename, sublist[0])) {
                        console.log("[matched]", filename);
                        if (retlist.length > 0) {
                            retlist.push({
                                token: "",
                                token_id: "Divider",
                                next: null,
                            });
                        }
                        retlist.push({
                            token: filename,
                            token_id: "Variable",
                            next: null,
                        });
                    }
                    i += 1;
                }
                if (retlist.length === 0) {
                    retlist.push(...sublist);
                } else if (subhead) {
                    retlist.push({
                        token: "",
                        token_id: "Divider",
                        next: null,
                    });
                }
            } else {
                // ファイル名展開不要
                retlist.push(...sublist);
            }

            // つぎつぎ
            sublist = [];
        }
        if (!subhead) { break; }
        subhead = subnext;
    }
    let k = 0;
    while (k < retlist.length - 1) {
        retlist[k].next = retlist[k + 1];
        k += 1;
    }
    return retlist.length > 0 ? retlist[0] : null;
}

function fetch_files(state: Parser.ParserState) {
    const v = EV.get_var(state.varmap, "FILE_NAMES");
    if (v) { return v.split(" "); }
    return [];
}

/**
 * 文字列nameがパターンsublistにマッチするかどうかを判定する
 */
function match_to_list(name: string, sublist: Subtoken) {
    let dp1: number[] = new Array(name.length + 1);
    let dp2: number[] = new Array(name.length + 1);
    let k = 0;
    while (k < name.length + 1) {
        dp1[k] = 0;
        dp2[k] = 0;
        k += 1;
    }
    dp1[0] = 1;
    // console.log(name, dp1, dp2);
    let subhead: Subtoken | null = sublist;
    while (subhead) {
        let i = 0;
        while (i < subhead.token.length) {
            const c = subhead.token[i];
            let j = 0;
            while (j < dp1.length) {
                // console.log(name, i, j, c, name[j], dp1[j])
                if (dp1[j]) {
                    if (subhead.token_id !== "DQuoted" && subhead.token_id !== "SQuoted" && c === "*") {
                        // - パターンの`i`文字目が`*`の時
                        // - 表の外に出ないなら、1つ下・1つ右・1つ右下の各セルに移動できる。
                        if (j + 1 < dp1.length) {
                            dp1[j + 1] = 1;
                            dp2[j + 1] = 1;
                        }
                        dp2[j] = 1;
                    } else if (c === name[j]) {
                        // パターンのi文字目と文字列のj文字目が一致するなら、1つ右下のセルに移動できる。
                        if (j + 1 < dp1.length) {
                            dp2[j + 1] = 1;
                        }
                    }
                }
                j += 1;
            }
            dp1.forEach((v, j) => {
                dp1[j] = dp2[j];
                dp2[j] = 0;
            })
            // console.log(name, i, dp1);
            i += 1;
        }
        subhead = subhead.next;
    }
    return dp1[dp1.length - 1];
}
