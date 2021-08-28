#ifndef MS_EXPANDER_H
# define MS_EXPANDER_H
# include "../libft/libft.h"
# include "ms_parser.h"

typedef enum e_ex_token_id
{
	XI_DUMMY,
	XI_DQUOTED, // ダブルクオーテッド
	XI_SQUOTED, // シングルクオーテッド
	XI_VAR, // $変数
	XI_BRACED_VAR, // ${変数}
	XI_BARE, // ネイキッド
	XI_WHT_SPACE, // 空白
	XI_NON_SPACE, // 非空白
	XI_SPLITTER, // 区切り(word splitting後, joining前のみ存在)
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

typedef struct	s_shellvar
{
	char				*key;
	char 				*value;
	int					is_env;
	int					attr;
	struct s_shellvar	*next;
}	t_shellvar;

typedef struct s_ex_state
{
	// 環境変数
	t_shellvar	*var;
	// 最後のコマンドの終了ステータス
	int			last_exit_status;
	t_pipeline	*pipeline;
	int			failed;

	int			no_split;
	int			ex_quoted; // すでにクオート内部であるかどうか
}	t_ex_state;

typedef struct s_ex_cursor
{
	// 材料 head
	t_ex_token		*src_head;
	// 材料 tail
	t_ex_token		*src_tail;
	// 生成物 head
	t_ex_token		*head;
	// 生成物 tail
	t_ex_token		*tail;
	t_ex_token_id	running;
	t_token_id		pa_token_id;
	size_t			i;
	size_t			n;
	size_t			vs;
	const char		*str;
}	t_ex_cursor;

// リダイレクションの展開
t_redir	*ms_expand_redir(t_ex_state *state, t_redir *redir);
// STreeの展開
// no_split: trueにするとワードスプリットを行わない
t_stree	*ms_expand_stree(t_ex_state *state, t_stree *stree);

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
void		ex_sp_open_any(t_ex_cursor *cursor);
int			ex_sp_open_bvar(t_ex_cursor *cursor);
int			ex_sp_close_quote(t_ex_state *state, t_ex_cursor *cursor);
int			ex_sp_close_var(t_ex_state *state, t_ex_cursor *cursor);
int			ex_sp_close_bare(t_ex_state *state, t_ex_cursor *cursor);

void	*ex_error(t_ex_state *state, t_stree *stree, char *message);
void	*ex_fatal(t_ex_state *state, char *message);
void	ex_destroy_token(t_ex_token *ext);

#endif