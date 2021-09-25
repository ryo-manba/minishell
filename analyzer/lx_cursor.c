/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_cursor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 02:12:20 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/25 12:09:14 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

void	lx_error_prologue(t_lex_cursor *cursor)
{
	ft_putstr_fd(cursor->master->prog_name, STDERR_FILENO);
	if (!cursor->master->interactive_shell)
	{
		if (cursor->master->opt_c)
			ft_putstr_fd(": -c", STDERR_FILENO);
		ft_putstr_fd(": line ", STDERR_FILENO);
		ft_putsizet_fd(cursor->master->line_num, STDERR_FILENO);
	}
	ft_putstr_fd(": ", STDERR_FILENO);
}

t_wdlist	*lx_unclosed_quote(t_lex_cursor *cursor)
{
	lx_error_prologue(cursor);
	ft_putstr_fd("unexpected EOF while looking for matching `", STDERR_FILENO);
	ft_putchar_fd(cursor->under_quoted, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	g_ex_states = 2;
	cursor->master->line_num += 1;
	lx_error_prologue(cursor);
	ft_putstr_fd("syntax error: unexpected end of file\n", STDERR_FILENO);
	lx_destroy_token(cursor->head);
	return (NULL);
}

int	lx_mark_failed(t_lex_cursor *cursor, int mark, char *message)
{
	if (mark)
	{
		cursor->failed = 1;
		if (!cursor->error_printed)
		{
			if (!message)
				ft_putstr_fd(MS_AZ_PROGNAME ": error in lexing\n",
					STDERR_FILENO);
			else
			{
				ft_putstr_fd(MS_AZ_PROGNAME ": ", STDERR_FILENO);
				ft_putstr_fd(message, STDERR_FILENO);
				ft_putstr_fd("\n", STDERR_FILENO);
			}
			cursor->error_printed = 1;
		}
	}
	return (cursor->failed);
}

t_wdlist	*lx_finalize(t_lex_cursor *cursor)
{
	if (cursor->failed)
	{
		lx_destroy_token(cursor->head);
		return (NULL);
	}
	else if (cursor->master->opt_c && cursor->under_quoted)
		return (lx_unclosed_quote(cursor));
	else
		lx_conclude_token(cursor);
	if (cursor->tail && cursor->tail->lex_type != LT_NEWLINE)
	{
		if (lx_add_token(cursor, 'N'))
		{
			lx_mark_failed(cursor, 1, "add lex-token(nl)");
			lx_destroy_token(cursor->head);
			return (NULL);
		}
		lx_conclude_token(cursor);
	}
	return (cursor->head);
}
