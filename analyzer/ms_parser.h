#ifndef MS_PARSER_H
# define MS_PARSER_H
# include "../libft/libft.h"
# include "ms_lexer.h"

typedef enum e_token_id
{
	TI_DUMMY,
	TI_WORD,
	TI_IO_NUMBER,
	TI_NAME,
	TI_ASSIGNMENT_WORD,
	TI_SUBSHELL,

	TI_LT,
	TI_GT,
	TI_LTLT,
	TI_GTGT,

	TI_LTGT,
	TI_LTAND,
	TI_GTAND,
	TI_LTLTHYPHEN,
	TI_PIPE,
	TI_ANDAND,
	TI_PIPEPIPE,
	TI_AND,
	TI_SEMICOLON,
	TI_PAREN_L,
	TI_PAREN_R,
	TI_NONE,
}	t_token_id;

struct	s_pipelinelist;

// clause内のトークンのリスト(木)
typedef struct s_stree
{
	char				*token;
	t_token_id			token_id;
	struct s_stree		*left;
	struct s_stree		*right;
	struct s_pipeline	*subshell;
	// expanderのjoiningにおいて、クオートされたextokenが1つでも含まれていれば1, そうでなければ0
	int					quote_involved;
}	t_stree;

// clause内のリダイレクションのリスト
typedef struct s_redir
{
	struct s_redir	*next;
	t_stree			*operand_left;
	t_stree			*operand_right;
	t_token_id		redir_op;
}	t_redir;

// clause
// リダイレクションとトークンを内包する
// 一つの独立したコマンドとして実行され、終了ステータスを持つ。
// (ただし、リダイレクションや導入)
typedef struct s_clause
{
	t_redir			*redir;
	t_stree			*stree;
	struct s_clause	*next;
}	t_clause;

// pipeline
// clauseを内包する
typedef struct s_pipeline
{
	t_clause			*clause;
	struct s_pipeline	*next;
	t_token_id			joint;
}	t_pipeline;

// parseの状態
// parseの進行状況に応じて変化させる
// cursor自身は自前でオブジェクトを所有しない。すべて借り物。
typedef struct s_parse_cursor
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
	t_wdlist		*words;
	t_pipeline		*pipeline;
	t_parse_cursor	cursor;
	int				for_subshell;

	int				finished;
	char			*err_message;
	t_wdlist		*err_word;
}	t_parse_state;

int			lx_is_an_operator(t_lex_cursor *cursor);
size_t		lx_cut_operator(t_lex_cursor *cursor);

int			ms_parse(t_parse_state *state, t_wdlist *words, int for_subshell);

t_token_id	pa_operator_token_id(t_wdlist *word);
const char	*pa_operator_label(t_token_id ti);
const char	*pa_token_label(t_token_id ti);
int			pa_unit(t_parse_state *state);
t_wdlist	*pa_shift_word(t_parse_state *state);
t_stree		*pa_make_stree(t_wdlist *word, int for_subshell);
t_redir		*pa_make_redir(t_wdlist *op_word, t_stree *target, t_stree *ion);
t_stree		*ex_make_stree(char *token, t_token_id tid);
void		pa_destroy_stree(t_stree *stree);
void		pa_destroy_redir(t_redir *redir);
void		pa_destroy_clause(t_clause *clause);
void		pa_destroy_pipeline(t_pipeline *pipeline);
t_stree		*pa_add_stree(t_parse_state *state, t_stree *stree);
t_redir		*pa_add_redir(t_parse_state *state, t_redir *redir);
t_clause	*pa_add_new_clause(t_parse_state *state);
t_pipeline	*pa_add_new_pipeline(t_parse_state *state);
int			pa_sub_enter_subshell(t_parse_state *state, t_wdlist *word);
int			pa_sub_leave_subshell(t_parse_state *state, t_wdlist *word);
int			pa_sub_redir(t_parse_state *state, t_wdlist *word, t_stree *ion_st);
int			pa_sub_term_pipeline(t_parse_state *state, t_wdlist *word);
int			pa_sub_term_clause(t_parse_state *state, t_wdlist *word);
char		*pa_syntax_final(t_parse_state *state);
int			pa_syntax_term_clause(t_parse_state *state, int by_newline);
int			pa_syntax_term_pipeline(t_parse_state *state, int by_newline);
int			pa_syntax_error(t_parse_state *state, t_wdlist *word,
				char *message);

#endif