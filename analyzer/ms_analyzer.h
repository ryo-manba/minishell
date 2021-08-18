#ifndef MS_ANALYZER_H
# define MS_ANALYZER_H
# include "../lexer/lexer.h"

typedef enum	e_token_id
{
	TI_DUMMY,
	TI_WORD,
	TI_IO_NUMBER,
	TI_NAME,
	TI_ASSIGNMENT_WORD,
	TI_SUBSHELL,
	// [リダイレクション演算子]
	TI_LT, // <
	TI_GT, // >
	TI_LTLT, // <<
	TI_GTGT, // >>
	TI_LTGT, // <>
	TI_LTAND, // <&
	TI_GTAND, // >&
	TI_LTLTHYPHEN, // <<-
	// [clause終端演算子]
	TI_PIPE, // |
	// [パイプライン終端演算子]
	TI_ANDAND, // &&
	TI_PIPEPIPE, // ||
	TI_AND, // &
	TI_SEMICOLON, // ;
	// [サブシェル開始・終了演算子]
	TI_PAREN_L, // (
	TI_PAREN_R, // )
	TI_NONE,
}	t_token_id;

struct	s_pipelinelist;

// clause内のトークンのリスト(木)
typedef struct	s_stree
{
    char					*token;
    t_token_id				token_id;
    struct s_stree			*left;
    struct s_stree			*right;
	// 親clauseがサブシェルの場合、これがNULLでなくなる
    struct s_pipelinelist	*subshell;
}	t_stree;

// clause内のリダイレクションのリスト
typedef struct	s_redir
{
	struct s_redir	*next;
	// あるとしたら、必ずIO_NUMBER
	t_stree			*operand_left;
	// 存在しないことがあるが、パーサの後では気にしなくていい。
	t_stree			operand_right;
	// 取りうる値は以下の通り:
	// TI_LT(<), TI_GT(>), TI_LTLT(<<), TI_GTGT(>>), TI_LTGT(<>), TI_LTAND(<&), TI_GTAND(>&), TI_LTLTHYPHEN(<<-)
    t_token_id			redir_op;
}	t_redir;

// clause
// リダイレクションとトークンを内包する
// 一つの独立したコマンドとして実行され、終了ステータスを持つ。
// (ただし、リダイレクションや導入)
typedef struct	s_clause
{
	// 存在しないことがある
	t_redir		*redir;
	// 存在しないことがある(たとえば、コマンド`> x`はパースエラーにならない。)
	t_stree		*stree;
	// なお、redirかstreeの少なくとも一方は存在している。

	struct s_clause	*next;
}	t_clause;

// pipeline
// clauseを内包する
typedef struct	s_pipeline
{
	t_clause			*clause;
	struct s_pipeline	*next;
	// 取りうる値は以下の通り:
	// TI_ANDAND(&&) TI_PIPEPIPE(||) TI_AND(&) TI_SEMICOLON(;) TI_NONE
	t_token_id			joint;
}	t_pipeline;

// pipelinelist
// pipelineを内包する
// サブシェルによってclause(stree)の直下に所属することがある
typedef struct	s_pipelinelist
{
	t_pipeline	*pipeline;
}	t_pipelinelist;

#endif