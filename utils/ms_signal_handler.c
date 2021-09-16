/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_signal_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/15 23:37:53 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/15 23:44:06 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

// ctrl+Cが来たら標準入力をcloseすることでreadlineはNULLを返す
void	ms_heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_ex_states = 1;
	ft_putchar_fd('\n', STDOUT_FILENO);
	close(STDIN_FILENO);
}

// ctrlCを判定するためにex_statesを0にしておく
int	ms_heredoc_signal_set(void)
{
	g_ex_states = 0;
	if (signal(SIGINT, ms_heredoc_sigint_handler) == SIG_ERR || \
		signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		ms_perror("signal");
		return (1);
	}
	return (0);
}
