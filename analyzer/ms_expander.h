#ifndef MS_EXPANDER_H
# define MS_EXPANDER_H
# include "../libft/libft.h"
# include "ms_parser.h"
# define EX_IFS	" \t\n"

typedef enum e_ex_token_id
{
	XI_DUMMY,
	XI_DQUOTED,
	XI_SQUOTED,
	XI_VAR,
	XI_BRACED_VAR,
	XI_BARE,
	XI_WHT_SPACE,
	XI_NON_SPACE,
	XI_SPLITTER,
	XI_NONE,
}	t_ex_token_id;

typedef struct s_ex_token
{
	const char			*token;
	t_ex_token_id		token_id;
	t_token_id			pa_token_id;
	struct s_ex_token	*right;
	struct s_ex_token	*left;
}	t_ex_token;

typedef struct s_shellvar
{
	char				*key;
	char				*value;
	int					is_env;
	int					attr;
	struct s_shellvar	*next;
}	t_shellvar;

typedef struct s_ex_state
{
	t_shellvar	*var;
	int			last_exit_status;
	t_pipeline	*pipeline;
	int			failed;

	int			no_split;
	int			ex_quoted;
}	t_ex_state;

typedef struct s_ex_part_cursor
{
	t_ex_token		*src_head;
	t_ex_token		*src_tail;
	t_ex_token		*head;
	t_ex_token		*tail;
}	t_ex_part_cursor;


typedef struct s_ex_cursor
{
	t_ex_part_cursor	p;
	t_ex_token_id		running;
	t_token_id			pa_token_id;
	size_t				i;
	size_t				n;
	size_t				vs;
	const char			*str;
}	t_ex_cursor;

// エグゼキュータがstateを初期化するための関数
// 呼び出し前は、stateは一切初期化しなくていい
// t_ex_state	state;
//
// ms_init_expander_state(&state, last_exit_status);
void		ms_init_expander_state(t_ex_state *state, int last_exit_status);
// redirの先頭のみをexpandする
t_redir		*ms_expand_redir(t_ex_state *state, t_redir *redir);
// streeの全体を先頭から順番にexpandする
t_stree		*ms_expand_stree(t_ex_state *state, t_stree *stree);


void		ex_add_token_csr(t_ex_part_cursor *cursor, t_ex_token *ext);
t_ex_token	*ex_pop_src_token_csr(t_ex_part_cursor *cursor);

t_ex_token	*ex_shell_param(t_ex_state *state, t_stree *stree);
t_ex_token	*ex_shell_param_unit(t_ex_state *state, t_token_id tid,
				const char *token);
t_ex_token	*ex_split_word(t_ex_state *state, t_ex_token *ext);
t_ex_token	*ex_filename(t_ex_state *state, t_ex_token *ext);
t_stree		*ex_join_words(t_ex_state *state, t_ex_token *ext);
t_ex_token	*ex_push_back_token(t_ex_state *state,
				t_ex_cursor *cursor, const char *given_str);

char		*ex_lstcat(t_ex_token *ext);
int			ex_replace_var(t_ex_state *state, t_ex_cursor *cursor);
void		ex_ll_open_any(t_ex_cursor *cursor);
int			ex_ll_open_bvar(t_ex_cursor *cursor);
int			ex_ll_close_quote(t_ex_state *state, t_ex_cursor *cursor);
int			ex_ll_close_var(t_ex_state *state, t_ex_cursor *cursor);
int			ex_ll_close_bare(t_ex_state *state, t_ex_cursor *cursor);

void		ex_init_sp_cursor(t_ex_cursor *cursor, t_token_id tid,
				const char *str);
void		ex_init_sw_cursor(t_ex_cursor *cursor, t_ex_token *ext);
void		*ex_error(t_ex_state *state, t_stree *stree, char *message);
void		*ex_fatal(t_ex_state *state, char *message);
void		ex_destroy_token(t_ex_token *ext);

#endif