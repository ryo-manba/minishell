/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_cursor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 02:12:20 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/06 02:43:34 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

int	lx_mark_failed(t_lex_cursor *cursor, int mark, char *message)
{
	if (mark)
	{
		cursor->failed = 1;
		if (!cursor->error_printed)
		{
			if (message)
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
		cursor->head = NULL;
	}
	else
		lx_conclude_token(cursor);
	return (cursor->head);
}
