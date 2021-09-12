/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:31:20 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/12 17:57:22 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

const char	g_type_chars[] = {
	LC_WORD,
	LC_NEWLINE,
	LC_SINGLE_QUOTE,
	LC_DOUBLE_QUOTE,
	LC_SPACE,
	LC_SPACE,
	LC_REDIRECT_INPUT,
	LC_REDIRECT_OUTPUT,
	LC_AND,
	LC_PIPE,
	LC_SEMICOLON,
	LC_PAREN_L,
	LC_PAREN_R,
	LC_BRACE_L,
	'\0'};

t_wdlist	*ms_lexer(const char *line, t_lex_cursor *cursor)
{
	char			chartype;
	int				i;

	ft_bzero(cursor, sizeof(t_lex_cursor));
	cursor->line = line;
	while (!cursor->failed && line[cursor->i])
	{
		i = ft_strchr_i("\n'\" \t<>&|;(){", line[cursor->i]);
		chartype = g_type_chars[i + 1];
		if (lx_treat_quote(cursor, line[cursor->i], chartype))
			continue ;
		else if (lx_treat_brace(cursor, line[cursor->i], chartype))
			continue ;
		else if (lx_treat_nl(cursor, line[cursor->i], chartype))
			break ;
		else if (lx_treat_space(cursor, line[cursor->i], chartype))
			continue ;
		else if (lx_treat_operator(cursor, line[cursor->i], chartype))
			continue ;
		cursor->i += 1;
	}
	return (lx_finalize(cursor));
}
