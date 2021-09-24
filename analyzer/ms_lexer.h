/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lexer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 01:39:11 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/25 00:51:28 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_LEXER_H
# define MS_LEXER_H

# include "../libft/libft.h"

# define LC_NEWLINE	'N'
# define LC_SINGLE_QUOTE	'\''
# define LC_DOUBLE_QUOTE	'"'
# define LC_SPACE	' '
# define LC_TAB	'\t'
# define LC_PIPE	'|'
# define LC_AND	'&'
# define LC_REDIRECT_INPUT	'<'
# define LC_REDIRECT_OUTPUT	'>'
# define LC_SEMICOLON	';'
# define LC_SEMICOLON	';'
# define LC_PAREN_L	'('
# define LC_PAREN_R	')'
# define LC_BRACE_L	'{'
# define LC_BRACE_R	'}'
# define LC_WORD	'_'
# define LX_OPERATOR_OPENER	"|&<>;()"

# define LX_ERR_GEN	257

typedef enum e_lex_type
{
	LT_DUMMY,
	LT_TOKEN,
	LT_IO_NUMBER,
	LT_OPERATOR,
	LT_NEWLINE,
}	t_lex_type;

typedef struct s_wdlist
{
	struct s_wdlist	*next;
	const char		*word;
	int				i;
	int				len;
	int				concluded;
	char			starting_chartype;
	char			delimiter;
	t_lex_type		lex_type;
}	t_wdlist;

typedef struct s_lex_cursor
{
	t_master	*master;
	t_wdlist	*head;
	t_wdlist	*tail;
	const char	*line;
	char		under_quoted;
	int			under_brace;
	int			i;
	int			escaping;
	int			failed;
	int			error_printed;
	int			bs[2];
}	t_lex_cursor;

# define CHARS_WORD_INCLUDED "\"'_{}"

t_wdlist	*ms_lexer(t_master *master, const char *line, t_lex_cursor *cursor);
int			lx_treat_operator(t_lex_cursor *cursor, char c, char ct);
int			lx_treat_space(t_lex_cursor *cursor, char c, char ct);
int			lx_treat_nl(t_lex_cursor *cursor, char c, char ct);
int			lx_treat_brace(t_lex_cursor *cursor, char c, char ct);
int			lx_treat_quote_and_word(t_lex_cursor *cursor, char c, char ct);
int			lx_add_token(t_lex_cursor *cursor, char starting_char);
void		lx_conclude_token(t_lex_cursor *cursor);
int			lx_str_is_digital(const	char *str, size_t len);
int			lx_char_is_for_name(char c, size_t pos);
int			lx_str_is_for_name(const char *str, size_t len);
int			lx_char_is_for_bare(char c);
int			lx_str_is_for_assignment_word(const char *str, size_t len);
int			lx_tail_is_an_operator(t_lex_cursor *cursor);
void		lx_destroy_token(t_wdlist *t);
int			lx_mark_failed(t_lex_cursor *cursor, int mark, char *message);
t_wdlist	*lx_finalize(t_lex_cursor *cursor);
int			lx_is_quoted(t_lex_cursor *cursor, char c);
void		lx_update_bs(t_lex_cursor *cursor, char c);
int			ms_is_quoted(int bs[2], int i, char c, char quoted);
void		ms_update_bs(int bs[2], int i, char c, char quoted);
int			lx_copy_op_table(t_master *master);
int			lx_copy_all_op_table(t_master *master);

#endif
