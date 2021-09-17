/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_state.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 21:42:54 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/18 02:59:06 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

void	ms_ex_init_state(t_ex_state *state, t_master *master, t_shellvar *env)
{
	ft_bzero(state, sizeof(t_ex_state));
	state->master = master;
	state->var = env;
}

int	ex_mark_failed(t_ex_state *state, int mark, char *message)
{
	if (mark)
	{
		state->failed = 1;
		if (!state->error_printed)
		{
			if (!message)
				ft_putstr_fd("-" MS_AZ_PROGNAME ": error in expansion\n",
					STDERR_FILENO);
			else
			{
				ft_putstr_fd("-" MS_AZ_PROGNAME ": ", STDERR_FILENO);
				ft_putstr_fd(message, STDERR_FILENO);
				ft_putstr_fd("\n", STDERR_FILENO);
			}
			state->error_printed = 1;
		}
	}
	return (state->failed);
}
