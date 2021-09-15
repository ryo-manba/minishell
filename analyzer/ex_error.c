/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 19:01:30 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/15 04:29:58 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

void	ex_error_prologue(t_ex_state *state)
{
	ft_putstr_fd(state->master->prog_name, STDERR_FILENO);
	if (!state->master->interactive_shell)
	{
		if (state->master->line_num > 0)
		{
			ft_putstr_fd(": line ", STDERR_FILENO);
			ft_putsizet_fd(state->master->line_num, STDERR_FILENO);
		}
	}
	ft_putstr_fd(": ", STDERR_FILENO);
}

void	*ex_error(t_ex_state *state, t_stree *stree, char *message)
{
	if (message && !state->error_printed && stree)
	{
		ex_error_prologue(state);
		ft_putstr_fd(stree->token, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(message, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		state->error_printed = 1;
	}
	ex_mark_failed(state, 1, message);
	return (NULL);
}
