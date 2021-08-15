import * as MS from "./minishell";
import * as Parser from "./parser";
import * as EV from "./envvar";

type SubtokenId = "DQuoted" | "SQuoted" | "Variable" | "Naked";
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
    while (i <= stree.token.length) {
        // 終了条件
        if (stat === "SQuoted" && i < stree.token.length && stree.token[i] === "'") {
            // SQuoted
            // (クオートされていない)'に遭遇すること。
            subhead = push_back_subtoken(stree.token.substring(var_start, i), stat, subhead);
            subtokens = subtokens || subhead;
            i += 1;
        } else if (stat === "DQuoted" && i < stree.token.length && stree.token[i] === "\"") {
            // DQuoted
            // (クオートされていない)"に遭遇すること。
            // TODO: Parameter Expansion
            subhead = push_back_subtoken(stree.token.substring(var_start, i), stat, subhead);
            subtokens = subtokens || subhead;
            i += 1;
        } else if (stat === "Variable" && var_start === i && stree.token[i] === "{") {
            // BracedVariableの**開始**
            // **Variable**状態で、`$`の次に(つまり`var_start === i`の時に)`{`に遭遇すること。
            var_start = i + 1;
            stat = "BracedVariable";
            i += 1;
            continue;
        } else if (stat === "BracedVariable" && stree.token[i] === "}") {
            // BracedVariableの**終了**
            // }に遭遇すること。
            const name = stree.token.substring(var_start, i);
            const val = EV.get_var(state.varmap, name);
            subhead = push_back_subtoken(val || "", "Variable", subhead);
            subtokens = subtokens || subhead;
            i += 1;
        } else if (stat === "Variable" && !MS.char_is_for_name(stree.token[i], i - var_start - 1)) {
            // Variable
            // NAME適格文字ではない文字または文字列終端に遭遇すること。
            const name = stree.token.substring(var_start, i);
            const val = EV.get_var(state.varmap, name);
            subhead = push_back_subtoken(val || "", stat, subhead);
            subtokens = subtokens || subhead;
        } else if (stat === "Naked" && !MS.char_is_for_naked(stree.token[i])) {
            // Naked
            // クオートされていない'"$または文字列終端に遭遇すること。
            // 長さが0になる場合はpushしない
            if (var_start < i) {
                subhead = push_back_subtoken(stree.token.substring(var_start, i), stat, subhead);
                subtokens = subtokens || subhead;
            }
        } else {
            // 何も終わらなかった
            i += 1;
            continue;
        }
        // 開始条件
        if (i === stree.token.length) {
            finished_expectedly = true;
            break;
        }
        if (stree.token[i] === "'") {
            // SQuoted
            // (クオートされていない)'に遭遇すること。
            var_start = i + 1;
            stat = "SQuoted"
        } else if (stree.token[i] === "\"") {
            // DQuoted
            // (クオートされていない)"に遭遇すること。
            var_start = i + 1;
            stat = "DQuoted"
        } else if (stree.token[i] === "$") {
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
    console.log(subtokens);
    return subtokens;
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
function expand_token_split(state: Parser.ParserState, stree: MS.STree) {

}
