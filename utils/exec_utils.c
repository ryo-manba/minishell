/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:46 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/27 11:08:13 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

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
