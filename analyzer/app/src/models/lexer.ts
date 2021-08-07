import * as MS from "./minishell";


/**
 * 文字種別を判定する
 */
function type_of_char(char: string) {
    if (char == MS.CHARTYPE_NEWLINE) { return char; }
    if (char == MS.CHARTYPE_SINGLE_QUOTE) { return char; }
    if (char == MS.CHARTYPE_DOUBLE_QUOTE) { return char; }
    if (char == MS.CHARTYPE_SPACE) { return char; }
    if (char == MS.CHARTYPE_TAB) { return MS.CHARTYPE_SPACE; }
    if (char == MS.CHARTYPE_PIPE) { return char; }
    if (char == MS.CHARTYPE_AND) { return char; }
    if (char == MS.CHARTYPE_REDIRECT_INPUT) { return char; }
    if (char == MS.CHARTYPE_REDIRECT_OUTPUT) { return char; }
    if (char == MS.CHARTYPE_SEMICOLON) { return char; }
    return MS.CHARTYPE_WORD;
}

/**
 * 与えられた文字が演算子構成文字かどうか
 */
function is_an_operator_char(
    c: string
) {
    return "|&<>-;".includes(c);
}

function is_a_operator(
    line: string,
    current_token: MS.WordList,
    j: number,
) {
    if (!current_token) { return null; }
    const str = line.substring(current_token.i, j);
    if (str == "<") { return MS.OP.REDIR_INPUT; }
    if (str == ">") { return MS.OP.REDIR_OUTPUT; }
    if (str == ">>") { return MS.OP.REDIR_APPEND; }
    if (str == "<>") { return MS.OP.REDIR_INANDOUT; }
    if (str == "<<") { return MS.OP.REDIR_HEREDOC; }
    if (str == "<<-") { return MS.OP.REDIR_HEREDOC_DETAB; }
    if (str == "|") { return MS.OP.PIPE; }
    if (str == "&&") { return MS.OP.AND_IF; }
    if (str == "||") { return MS.OP.OR_IF; }
    if (str == "&") { return MS.OP.BACKGROUND; }
    if (str == ";") { return MS.OP.SEMICOLON; }
    if (str == "<&") { return MS.OP.DUPFD_IN; }
    if (str == ">&") { return MS.OP.DUPFD_OUT; }
    return null;
}


// 新しいトークンを開始する
// 現在のトークンが終了していない場合は何もしない
function add_lexer_token(
    current_token: MS.WordList | null,
    i: number,
    starting_chartype: string,
) {
    if (current_token && !current_token.concluded) { return current_token; }
    const next: MS.WordList = {
        next: null,
        word: "",
        detail_type: 0,
        token_type: 0,
        i: i,
        start: -1,
        concluded: current_token ? 0 : 1,
        starting_chartype,
    };
    if (current_token) {
        current_token.next = next;
    }
    return next;
}

// 現在のトークンを終了する
// すでに終了している場合は何もしない
function conclude_lexer_token(
    line: string,
    current_token: MS.WordList,
    j: number,
) {
    if (current_token.concluded) { return; }
    current_token.concluded = 1;
    current_token.word = line.substring(current_token.i, j);
    // TODO: IO_NUMBERかどうかのテスト
}

export function lexer(line: string): MS.WordList {
    const root_list = add_lexer_token(null, -1, "");
    let tail_list = root_list;
    let under_quote: string = "";
    let i = 0;
    while (i < line.length) {
        const char_type = type_of_char(line[i]);
        if (under_quote && under_quote == char_type) {
            // [クオート解除]
            under_quote = "";
        } else if (char_type == MS.CHARTYPE_SINGLE_QUOTE) {
            // [シングルクオート]
            under_quote = char_type;
            // トークンが開始していない場合は開始
            tail_list = add_lexer_token(tail_list, i, char_type);
        } else if (char_type == MS.CHARTYPE_DOUBLE_QUOTE) {
            // [ダブルクオート]
            under_quote = char_type;
            // トークンが開始していない場合は開始
            tail_list = add_lexer_token(tail_list, i, char_type);
        } else if (under_quote || char_type == MS.CHARTYPE_WORD) {
            // [クオート下にあるクオートを解除しない文字]
            // or
            // [クオート下にない単語構成文字]
            // カレントトークンが単語でないならトークンを終了
            if (!MS.CHARTYPESET.WORD_INCLUDED.includes(tail_list.starting_chartype)) {
                conclude_lexer_token(line, tail_list, i);
            }
            tail_list = add_lexer_token(tail_list, i, char_type);
            // [[ 以下全てクオート下でない ]]
        } else if (char_type == MS.CHARTYPE_NEWLINE) {
            // [改行文字]
            // トークンを終了し、新しいトークンを開始した後解析を終了する。
            conclude_lexer_token(line, tail_list, i);
            tail_list = add_lexer_token(tail_list, i, char_type);
            i += 1;
            break;
        } else if (char_type == MS.CHARTYPE_SPACE) {
            // 空白文字 -> トークンが終了していなければトークンを終了する
            conclude_lexer_token(line, tail_list, i);
        } else if (is_an_operator_char(char_type)) {
            console.log(i, line[i], char_type);
            // 演算子構成文字、つまり | & < > - ; のどれか
            if (tail_list.concluded) {
                // - トークンが終了しているなら開始
                tail_list = add_lexer_token(tail_list, i, char_type);
            } else {
                // - そうでないなら、トークンの開始文字が演算子構成文字かどうかをチェック
                if (is_an_operator_char(tail_list.starting_chartype)) {
                    // さらに、トークンの開始文字からこの文字までを含めた部分文字列が演算子をなすかどうかをチェック
                    const operator_type = is_a_operator(line, tail_list, i + 1);
                    console.log(i, line[i], operator_type);
                    if (operator_type) {
                        // 演算子をなすなら、この文字までをトークンに含める
                    } else {
                        conclude_lexer_token(line, tail_list, i);
                        tail_list = add_lexer_token(tail_list, i, char_type);
                    }
                } else {
                    // - そうでないなら、ここでトークンを普通に切り、トークンを開始
                    conclude_lexer_token(line, tail_list, i);
                    tail_list = add_lexer_token(tail_list, i, char_type);
                }
            }
        }
        i += 1;
    }
    // トークンが終わってなければ終わらせる
    conclude_lexer_token(line, tail_list, i);
    return root_list;
}

