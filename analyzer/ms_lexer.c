/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:31:20 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/25 01:31:32 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

static char	lx_special_index(const char *line, t_lex_cursor *cursor)
{
	ssize_t	i;

	if (lx_is_quoted(cursor, line[cursor->i]))
		return (LC_WORD);
	i = ft_strchr_i("\n'\" \t<>&|;(){", line[cursor->i]);
	return ((char []){
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
	}[i + 1]);
}

t_wdlist	*ms_lexer(t_master *master, const char *line, t_lex_cursor *cursor)
{
	char			chartype;

	ft_bzero(cursor, sizeof(t_lex_cursor));
	cursor->master = master;
	cursor->line = line;
	while (!cursor->failed && line[cursor->i])
	{
		chartype = lx_special_index(line, cursor);
		if (lx_treat_quote_and_word(cursor, line[cursor->i], chartype))
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
