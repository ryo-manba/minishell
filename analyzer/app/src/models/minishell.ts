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
export const CHARTYPE_PAREN_L = '(';
export const CHARTYPE_PAREN_R = ')';
export const CHARTYPE_WORD = '_';
export const CHAR_SPACELIKE = [CHARTYPE_SPACE, CHARTYPE_TAB];

export const CHARTYPESET = {
    WORD_INCLUDED: "\"'_",
};

export type WORD_LEX_TYPE = "TOKEN" | "IO_NUMBER" | "OPERATOR" | "NEWLINE";

export const RedirectionOperators = ["<", ">", "<<", ">>", "<>", "<&", ">&", "<<-" ] as const;
type TokenRedirectionOperator = typeof RedirectionOperators[number];

export const ClauseTerminateOperators = ["|"] as const;
type ClauseTerminateOperator = typeof ClauseTerminateOperators[number];

export const PipelineTerminateOperators = ["&&", "||",  "&", ";"] as const;
type PipelineTerminateOperator = typeof PipelineTerminateOperators[number];

export const SubshellOpenOperators = ["("] as const;
type SubshellOpenOperator = typeof SubshellOpenOperators[number];

export const SubshellCloseOperators = [")"] as const;
type SubshellCloseOperator = typeof SubshellCloseOperators[number];

type TokenOperator = SubshellOpenOperator | SubshellCloseOperator | ClauseTerminateOperator | PipelineTerminateOperator | TokenRedirectionOperator;
type TokenIdentifier = "WORD" | "IO_NUMBER" | "NAME" | "ASSIGNMENT_WORD" | "SUBSHELL" | TokenOperator;

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
    PAREN_L: 81,
    PAREN_R: 82,
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

/**
 * リダイレクション演算子
 */
export type RedirList = {
    /**
     * 次の要素
     */
    next: RedirList | null;
    /**
     * 左オペランド
     */
    operand_left: STree | null;
    /**
     * 右オペランド
     */
    operand_right: WordList;
    /**
     * リダイレクション演算子種別
     */
    op: TokenRedirectionOperator;
};

export type STree = {
    /**
     * トークン文字列
     */
    token: string;
    /**
     * トークン識別子
     */
    token_id: TokenIdentifier;
    /**
     * このノードまでのツリー深度
     */
    depth: number;
    /**
     * 左ノード
     */
    left: STree | null;
    /**
     * 右ノード
     */
    right: STree | null;
    /**
     * この要素がサブシェルなら、サブシェル(PipelineList)への参照を持つ。
     */
    subshell: PipelineList | null;
    
};

export type Clause = {
    redirs: RedirList | null;
    stree: STree | null;
    next: Clause | null;
};

export type Pipeline = {
    clause: Clause;
    next: Pipeline | null;
    joint: PipelineTerminateOperator | null;
};

export type PipelineList = {
    pipeline: Pipeline;
}

/**
 * シェル変数
 */
export type ShellVariable = {
    key: string;
    value: string | null;
    is_env: boolean;
    attr: number;
};


