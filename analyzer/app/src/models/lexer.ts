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
    // 以下、演算子開始文字 <>&|;()
    if (char == MS.CHARTYPE_REDIRECT_INPUT) { return char; }
    if (char == MS.CHARTYPE_REDIRECT_OUTPUT) { return char; }
    if (char == MS.CHARTYPE_AND) { return char; }
    if (char == MS.CHARTYPE_PIPE) { return char; }
    if (char == MS.CHARTYPE_SEMICOLON) { return char; }
    if (char == MS.CHARTYPE_PAREN_L) { return char; }
    if (char == MS.CHARTYPE_PAREN_R) { return char; }
    return MS.CHARTYPE_WORD;
}

/**
 * 与えられた文字が演算子開始文字かどうか
 */
function is_an_operator_start_char(
    c: string
) {
    return "|&<>;()".includes(c);
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
    if (str == "(") { return MS.OP.PAREN_L; }
    if (str == ")") { return MS.OP.PAREN_R; }
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
        i: i,
        concluded: current_token ? 0 : 1,
        starting_chartype,
        right_delimiter: "",
        lex_type: "TOKEN",
    };
    if (current_token) {
        current_token.next = next;
    }
    return next;
}

function is_digital_str(str: string) {
    return !!str.match(/^\d+$/);
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
    if (j < line.length) {
        // 区切り文字をセット
        current_token.right_delimiter = line[j];
    }
    // lex_typeの設定
    // 初期値はTOKEN
    if (current_token.starting_chartype == MS.CHARTYPE_NEWLINE) {
        // 改行はトークン識別子NEWLINEになる。
        current_token.lex_type = "NEWLINE";
    } else if (is_a_operator(line, current_token, j)) {
        // 演算子トークンは、その演算子トークンに対応するトークン識別子になる。
        current_token.lex_type = "OPERATOR";
    } else if ("<>".includes(current_token.right_delimiter) && is_digital_str(current_token.word)) {
        // 区切り文字が<>である数字のみのトークンは トークン識別子IO_NUMBER となる。
        current_token.lex_type = "IO_NUMBER"
    }
}

export function lexer(line: string): MS.WordList {
    const root_token = add_lexer_token(null, -1, "");
    let current_token = root_token;
    let under_quote: string = "";
    let i = 0;
    while (i < line.length) {
        const char_type = type_of_char(line[i]);
        if (under_quote && under_quote == char_type) {
            // [クオート解除]
            under_quote = "";
        } else if (!under_quote && char_type == MS.CHARTYPE_SINGLE_QUOTE) {
            // [シングルクオート]
            under_quote = char_type;
            // トークンが開始していない場合は開始
            current_token = add_lexer_token(current_token, i, char_type);
        } else if (!under_quote && char_type == MS.CHARTYPE_DOUBLE_QUOTE) {
            // [ダブルクオート]
            under_quote = char_type;
            // トークンが開始していない場合は開始
            current_token = add_lexer_token(current_token, i, char_type);
        } else if (under_quote || char_type == MS.CHARTYPE_WORD) {
            // [クオート下にあるクオートを解除しない文字]
            // or
            // [クオート下にない単語構成文字]
            // カレントトークンが単語でないならトークンを終了
            if (!MS.CHARTYPESET.WORD_INCLUDED.includes(current_token.starting_chartype)) {
                conclude_lexer_token(line, current_token, i);
            }
            current_token = add_lexer_token(current_token, i, char_type);
            // [[ 以下全てクオート下でない ]]
        } else if (char_type == MS.CHARTYPE_NEWLINE) {
            // [改行文字]
            // トークンを終了し、新しいトークンを開始した後解析を終了する。
            conclude_lexer_token(line, current_token, i);
            current_token = add_lexer_token(current_token, i, char_type);
            i += 1;
            break;
        } else if (char_type == MS.CHARTYPE_SPACE) {
            // [空白文字]
            // トークンが終了していなければトークンを終了する
            conclude_lexer_token(line, current_token, i);
        } else if (is_an_operator_start_char(char_type)) {
            // [演算子構成文字、つまり <>&|;() のどれか]
            if (current_token.concluded) {
                // - トークンが終了しているなら開始
                current_token = add_lexer_token(current_token, i, char_type);
            }
            // - そうでないなら、トークンの開始文字が演算子開始文字かどうかをチェック
            if (!is_an_operator_start_char(current_token.starting_chartype)) {
                // - そうでないなら、ここでトークンを普通に切り、トークンを開始
                conclude_lexer_token(line, current_token, i);
                current_token = add_lexer_token(current_token, i, char_type);
            }
            // トークンの開始文字が演算子開始文字なら、ここから最も長い演算子を切り出す。
            if (char_type == "<") {
                i = cut_operator_le(line, i);
            } else if (char_type == ">") {
                i = cut_operator_ge(line, i);
            } else if (char_type == "&") {
                i = cut_operator_and(line, i);
            } else if (char_type == "|") {
                i = cut_operator_pipe(line, i);
            } else if (char_type == ";") {
                i = cut_operator_semicolon(line, i);
            } else if (char_type == "(") {
                i = cut_operator_paren_l(line, i);
            } else if (char_type == ")") {
                i = cut_operator_paren_r(line, i);
            }
            conclude_lexer_token(line, current_token, i);
            continue;
        }
        i += 1;
    }
    // トークンが終わってなければ終わらせる
    conclude_lexer_token(line, current_token, i);
    return root_token;
}

/**
 * `line[i]`から"<"で始まる最も長い演算子が取れるまで`i`を進める
 */
function cut_operator_le(line: string, i: number) {
    i += 1;
    if (line[i] == "<") {
        i += 1;
        if (line[i] == "-") {
            i += 1;
        }
    } else if (line[i] == ">") {
        i += 1;
    } else if (line[i] == "&") {
        i += 1;
    }
    return i;
}

/**
 * `line[i]`から">"で始まる最も長い演算子が取れるまで`i`を進める
 */
function cut_operator_ge(line: string, i: number) {
    i += 1;
    if (line[i] == ">") {
        i += 1;
    } else if (line[i] == "&") {
        i += 1;
    }
    return i;
}

/**
 * `line[i]`から"&"で始まる最も長い演算子が取れるまで`i`を進める
 */
function cut_operator_and(line: string, i: number) {
    i += 1;
    if (line[i] == "&") {
        i += 1;
    }
    return i;
}

/**
 * `line[i]`から"|"で始まる最も長い演算子が取れるまで`i`を進める
 */
function cut_operator_pipe(line: string, i: number) {
    i += 1;
    if (line[i] == "|") {
        i += 1;
    }
    return i;
}

/**
 * `line[i]`から";"で始まる最も長い演算子が取れるまで`i`を進める
 */
function cut_operator_semicolon(line: string, i: number) {
    i += 1;
    if (line[i] == ";") {
        i += 1;
    }
    return i;
}

/**
 * `line[i]`から"("で始まる最も長い演算子が取れるまで`i`を進める
 */
 function cut_operator_paren_l(line: string, i: number) {
    i += 1;
    return i;
}

/**
 * `line[i]`から")"で始まる最も長い演算子が取れるまで`i`を進める
 */
 function cut_operator_paren_r(line: string, i: number) {
    i += 1;
    return i;
}
