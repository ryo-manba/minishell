/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:46 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/10/01 17:33:08 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

int	exec_simple_for_subshell(t_clause *clause, t_stree *expanded)
{
	if (clause->stree && clause->stree->subshell)
	{
		pa_destroy_stree(expanded);
		return (1);
	}
	return (0);
}

void	exec_set_signal_wait(pid_t pid)
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		ms_perror_exit("signal");
	exec_update_exitstatus(pid);
	while (wait(NULL) > 0)
		;
	if (signal(SIGINT, ms_sigint_handler) == SIG_ERR)
		ms_perror_exit("signal");
}

int	exec_out(int rv, t_stree *expanded)
{
	pa_destroy_stree(expanded);
	return (rv);
}
