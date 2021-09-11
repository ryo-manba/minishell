/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_sighandler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 16:34:48 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/11 16:52:32 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

extern volatile sig_atomic_t	g_ex_states;

void	exec_sighandler(int sig)
{
	g_ex_states = sig + 128;
	printf("%d\n", g_ex_states);
	exit(g_ex_states);

}
// 子プロセスの場合
int	exec_set_sighandler()
{
	if (signal(SIGINT, exec_sighandler) == SIG_ERR || \
		signal(SIGQUIT, exec_sighandler) == SIG_ERR)
		print_perror_exit("signal");
}
