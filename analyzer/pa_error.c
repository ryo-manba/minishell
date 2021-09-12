/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pa_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:20:12 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/12 19:07:18 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

int	pa_generic_error(t_parse_state *state, t_wdlist *word, char *message)
{
	state->err_word = word;
	return (pa_mark_failed(state, PA_ERR_GENERIC, message));
}

int	pa_syntax_error(t_parse_state *state, t_wdlist *word, char *message)
{
	state->err_word = word;
	if (word && word->word && !state->error_printed)
	{
		ft_putstr_fd(MS_AZ_PROGNAME ": syntax error near unexpected token `",
			STDERR_FILENO);
		if (word->lex_type == LT_NEWLINE)
			ft_putstr_fd("newline", STDERR_FILENO);
		else
			ft_putnstr_fd((char *)word->word, word->len, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		if (PA_DEBUG && message)
		{
			ft_putstr_fd(message, STDERR_FILENO);
			ft_putstr_fd("\n", STDERR_FILENO);
		}
		state->error_printed = 1;
	}
	return (pa_mark_failed(state, PA_ERR_UNEXPECTED, message));
}

int	pa_mark_failed(t_parse_state *state, int mark, char *message)
{
	if (mark)
	{
		state->failed = mark;
		if (!state->error_printed)
		{
			if (!message)
				ft_putstr_fd(MS_AZ_PROGNAME ": error in parsing\n",
					STDERR_FILENO);
			else
			{
				ft_putstr_fd(MS_AZ_PROGNAME ": ", STDERR_FILENO);
				ft_putstr_fd(message, STDERR_FILENO);
				ft_putstr_fd("\n", STDERR_FILENO);
			}
			state->error_printed = 1;
		}
	}
	return (state->failed);
}
