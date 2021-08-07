export const CHARTYPE_NEWLINE = '\n';
export const CHARTYPE_SINGLE_QUOTE = "'";
export const CHARTYPE_DOUBLE_QUOTE = '"';
export const CHARTYPE_SPACE = ' ';
export const CHARTYPE_TAB = `\t`;
export const CHARTYPE_PIPE = '|';
export const CHARTYPE_AND = '&';
export const CHARTYPE_REDIRECT_INPUT = '<';
export const CHARTYPE_REDIRECT_OUTPUT = '>';
export const CHARTYPE_SEMICOLON = ';';
export const CHARTYPE_WORD = '_';
export const CHAR_SPACELIKE = [CHARTYPE_SPACE, CHARTYPE_TAB];

export const CHARTYPESET = {
    WORD_INCLUDED: "\"'_",
};

export type WORD_LEX_TYPE = "TOKEN" | "IO_NUMBER" | "OPERATOR" | "NEWLINE";

export const OP = {
    REDIR_INPUT: 1,
    REDIR_OUTPUT: 2,
    REDIR_APPEND: 3,
    REDIR_INANDOUT: 4,
    REDIR_HEREDOC: 5,
    REDIR_HEREDOC_DETAB: 6,
    PIPE: 11,
    AND_IF: 21,
    OR_IF: 22,
    BACKGROUND: 31,
    SEMICOLON: 51,
    DUPFD_IN: 61,
    DUPFD_OUT: 62,
};

export type WordList = {
    /**
     * 次の要素
     */
    next: WordList | null;
    /**
     * トークン文字列
     */
    word: string;
    /**
     * 入力文字列における開始インデックス
     */
    i: number;
    /**
     * トークンが終了しているか？
     */
    concluded: number;
    /**
     * 開始文字の文字タイプ
     */
    starting_chartype: string;
    /**
     * トークン最終文字の右隣の文字
     */
    right_delimiter: string;
    /**
     * Lexer時点で決まるラフなタイプ
     */
    lex_type: WORD_LEX_TYPE;
};

