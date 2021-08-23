#ifndef MS_ANALYZER_H
# define MS_ANALYZER_H
# include "../libft/libft.h"
# include "ms_lex.h"

# define MS_AZ_SUCC 0
# define MS_AZ_FAIL 1

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
	t_stree			*operand_right;
	// 取りうる値は以下の通り:
	// TI_LT(<), TI_GT(>), TI_LTLT(<<), TI_GTGT(>>), TI_LTGT(<>), TI_LTAND(<&), TI_GTAND(>&), TI_LTLTHYPHEN(<<-)
    t_token_id		redir_op;
}	t_redir;

// clause
// リダイレクションとトークンを内包する
// 一つの独立したコマンドとして実行され、終了ステータスを持つ。
// (ただし、リダイレクションや導入)
typedef struct	s_clause
{
	// 存在しないことがある
	t_redir			*redir;
	// 存在しないことがある(たとえば、コマンド`> x`はパースエラーにならない。)
	t_stree			*stree;
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

typedef struct	s_shellvar
{
	char	*key;
	char 	*value;
	int		is_env;
    int		attr;
}	t_shellvar;

// parseの状態
// parseの進行状況に応じて変化させる
typedef struct	s_parse_cursor
{
	t_pipeline	*pipeline;
	t_clause	*clause;
	t_redir		*redir;
	t_stree		*stree;
	t_wdlist	*word;
	int			expecting_continuation;
}	t_parse_cursor;

typedef struct s_parse_state
{
	t_wdlist		*words; // こっちは代入後に変更しない
	t_pipelinelist	pipelinelist;
	t_parse_cursor	cursor;
	int				for_subshell;

	int				finished;
	char			*error_message;
	t_wdlist		*error_word;
}	t_parse_state;


size_t	ms_cut_operator(t_lex_cursor *cursor);
t_token_id	ms_operator_token_id(t_wdlist *word);
const char	*ms_operator_label(t_token_id ti);
const char	*ms_token_label(t_token_id ti);
int	ms_init_parse_state(t_parse_state *state, t_wdlist *words, int for_subshell);
int	ms_parse(t_parse_state *state);
int	ms_parse_unit(t_parse_state *state);
t_wdlist	*ms_shift_word(t_parse_state *state);
int	ms_return_with_error(t_parse_state *state, t_wdlist *word, char *message);
t_stree	*ms_make_stree(t_wdlist *word);
t_redir	*ms_make_redir(t_wdlist *op_word, t_stree *target, t_stree *ion);
t_stree	*ms_parse_add_stree(t_parse_state *state, t_stree *stree);
t_redir	*ms_parse_add_redir(t_parse_state *state, t_redir *redir);
t_clause	*ms_parse_add_new_clause(t_parse_state *state);
t_pipeline	*ms_parse_add_new_pipeline(t_parse_state *state);
int	ms_subparse_redir(t_parse_state *state, t_wdlist *word, t_stree *ion_st);
int	ms_subparse_subshell(t_parse_state *state, t_wdlist *word);
int ms_subparse_term_pipeline(t_parse_state *state, t_wdlist *word);
int ms_subparse_term_clause(t_parse_state *state, t_wdlist *word);
int	ms_syntax_term_clause(t_parse_state *state, int by_newline);
int	ms_syntax_term_pipeline(t_parse_state *state, int by_newline);

#endif