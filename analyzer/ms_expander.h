/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expander.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 01:38:23 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/07 12:51:16 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_EXPANDER_H
# define MS_EXPANDER_H
# include "../libft/libft.h"
# include "ms_parser.h"
# include <dirent.h>
# define EX_IFS	" \t\n"
# define EX_SPECIAL_VAR_CHAR "?"

typedef unsigned char	*t_ucp;

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
	XI_DIVIDER,
	XI_NEUTRAL,
}	t_ex_token_id;

typedef struct s_ex_token
{
	const char			*token;
	t_ex_token_id		token_id;
	t_token_id			pa_token_id;
	struct s_ex_token	*right;
	struct s_ex_token	*left;
	size_t				n;
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
	int			error_printed;

	int			no_param;
	int			no_file;
	int			no_split;
	int			ex_quoted;
	t_ex_token	*file_names;
}	t_ex_state;

typedef struct s_ex_part_cursor
{
	t_ex_token	*head;
	t_ex_token	*tail;
}	t_ex_part_cursor;

typedef struct s_ex_st_cursor
{
	t_stree	*head;
	t_stree	*tail;
}	t_ex_st_cursor;

typedef struct s_ex_unit_cursor
{
	t_ex_part_cursor	s;
	t_ex_part_cursor	p;
	t_ex_st_cursor		t;
	t_ex_token_id		running;
	t_token_id			pa_token_id;
	size_t				i;
	size_t				n;
	size_t				vs;
	const char			*str;
	size_t				substr_s;
	size_t				substr_e;
	char				quote;
}	t_ex_unit_cursor;

typedef struct s_ex_cursor
{
	t_ex_st_cursor	src;
	t_ex_st_cursor	res;
}	t_ex_cursor;

typedef struct s_ex_fx_dpcursor
{
	t_ucp			dp[2];
	char			*pattern;
	size_t			n;
	int				match_hidden;
	size_t			mathched;
}	t_ex_fx_dpcursor;

void		ms_init_expander_state(t_ex_state *state, t_shellvar *env,
				int last_exit_status);
t_redir		*ms_expand_a_redir(t_ex_state *state, t_redir *redir);
t_stree		*ms_expand_stree(t_ex_state *state, t_stree *stree);

void		ex_add_token_csr(t_ex_part_cursor *cursor, t_ex_token *ext);
t_ex_token	*ex_pop_src_token_csr(t_ex_part_cursor *cursor);

void		ms_ex_init_state(t_ex_state *state, t_shellvar *env,
				int last_exit_status);
t_ex_token	*ex_shell_param(t_ex_state *state, t_stree *stree);
t_ex_token	*ex_split(t_ex_state *state, t_ex_token *token);
t_ex_token	*ex_fx(t_ex_state *state, t_ex_token *token);
t_ex_token	*ex_filename(t_ex_state *state, t_ex_token *ext);
t_stree		*ex_join(t_ex_state *state, t_ex_token *ext);
t_ex_token	*ex_push_back_token(t_ex_state *state,
				t_ex_unit_cursor *cursor, const char *given_str);

char		*ex_lstcat(t_ex_token *ext);
void		ex_init_cursor_mid(t_ex_unit_cursor *cursor, t_ex_token *ext);

void		ex_ll_unit(t_ex_state *state, t_ex_unit_cursor *csr);
int			ex_ll_trap_squoted(t_ex_state *state, t_ex_unit_cursor *csr);
int			ex_ll_trap_dquote(t_ex_state *state, t_ex_unit_cursor *csr);
int			ex_ll_trap_var(t_ex_state *state, t_ex_unit_cursor *csr);
int			ex_ll_trap_braced_var(t_ex_state *state, t_ex_unit_cursor *csr);
int			ex_ll_trap_bare(t_ex_state *state, t_ex_unit_cursor *csr);
int			ex_ll_replace_var(t_ex_state *state, t_ex_unit_cursor *csr);
char		*ex_strcat_exlist(t_ex_token *head, size_t s);
void		ex_ll_init_cursor(t_ex_unit_cursor *cursor, t_token_id tid,
				const char *str, char quote);
int			ex_push_back_divider_if_needed(t_ex_state *state,
				t_ex_unit_cursor *csr, t_ex_token *token);
t_ex_token	*ex_fx_dir_ents(t_ex_state *state);
size_t		ex_fx_expand(t_ex_state *state, t_ex_unit_cursor *cursor,
				char *pattern, size_t n);
t_ex_token	*ex_clone_and_push_back_token(t_ex_state *state,
				t_ex_unit_cursor *csr, t_ex_token *token);

void		*ex_error(t_ex_state *state, t_stree *stree, char *message);
int			ex_mark_failed(t_ex_state *state, int mark, char *message);
void		ex_destroy_token(t_ex_token *ext);
void		ex_destroy_a_token(t_ex_token *ext);

void		ex_stringify_extoken(t_ex_token *ext);

#endif