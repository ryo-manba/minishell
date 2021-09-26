/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parser.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 01:40:02 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/26 12:38:19 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_PARSER_H
# define MS_PARSER_H
# include "../common/ms_common.h"
# include "../libft/libft.h"
# include "ms_lexer.h"
# ifndef PA_DEBUG
#  define PA_DEBUG 0
# endif
# define PA_ERR_GENERIC	257
# define PA_ERR_UNEXPECTED	258

struct	s_pipelinelist;

typedef struct s_stree
{
	char				*token;
	t_token_id			token_id;
	struct s_stree		*left;
	struct s_stree		*right;
	struct s_pipeline	*subshell;
	int					quote_involved;
}	t_stree;

typedef struct s_redir
{
	struct s_redir	*next;
	t_stree			*operand_left;
	t_stree			*operand_right;
	int				heredoc_fd;
	t_token_id		redir_op;
}	t_redir;

// clause
typedef struct s_clause
{
	t_redir			*redir;
	t_stree			*stree;
	struct s_clause	*next;
}	t_clause;

// pipeline
typedef struct s_pipeline
{
	t_clause			*clause;
	struct s_pipeline	*next;
	t_token_id			joint;
}	t_pipeline;

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
	t_master		*master;
	t_wdlist		*words;
	t_pipeline		*pipeline;
	t_parse_cursor	cursor;
	int				for_subshell;

	int				finished;
	int				failed;
	int				error_printed;
	char			*err_message;
	t_wdlist		*err_word;
}	t_parse_state;

size_t		lx_cut_operator(t_lex_cursor *cursor);

int			ms_parse(t_master *master, t_parse_state *state, t_wdlist *words,
				int for_subshell);

t_token_id	pa_operator_token_id(t_parse_state *state, t_wdlist *word);
const char	*pa_operator_label(t_master *master, t_token_id ti);
const char	*pa_token_label(t_master *master, t_token_id ti);
int			pa_unit(t_parse_state *state);
t_wdlist	*pa_shift_lx_token(t_parse_state *state);
t_stree		*pa_make_stree(t_wdlist *word, int for_subshell);
t_redir		*pa_make_redir(t_parse_state *state,
				t_wdlist *op_word, t_stree *target, t_stree *ion);
t_stree		*ex_make_stree(char *token, t_token_id tid);
void		pa_destroy_stree(t_stree *stree);
void		pa_destroy_redir(t_redir *redir);
void		pa_destroy_clause(t_clause *clause);
void		pa_destroy_pipeline(t_pipeline *pipeline);
t_stree		*pa_add_stree(t_parse_state *state, t_stree *stree);
t_redir		*pa_add_redir(t_parse_state *state, t_redir *redir);
t_clause	*pa_add_new_clause(t_parse_state *state);
t_pipeline	*pa_add_new_pipeline(t_parse_state *state);
int			pa_subshell_enter(t_parse_state *state, t_wdlist *word);
int			pa_subshell_leave(t_parse_state *state, t_wdlist *word);
int			pa_redirection(t_parse_state *state, t_wdlist *word,
				t_stree *ion_st);
int			pa_terminate_pipeline(t_parse_state *state, t_wdlist *word);
int			pa_terminate_clause(t_parse_state *state, t_wdlist *word);
char		*pa_syntax_final(t_parse_state *state);
int			pa_syntax_term_clause(t_parse_state *state, int by_newline);
int			pa_syntax_term_pipeline(t_parse_state *state, int by_newline);
int			pa_syntax_error(t_parse_state *state, t_wdlist *word,
				char *message);
int			pa_generic_error(t_parse_state *state, t_wdlist *word,
				char *message);
int			pa_mark_failed(t_parse_state *state, int mark, char *message);

#endif
