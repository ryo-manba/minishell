/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main_flow.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:54 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/27 11:01:20 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

void	exec_update_exitstatus(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit: 3", STDERR_FILENO);
		else if (WTERMSIG(status) == SIGINT)
			ft_putchar_fd('\n', STDERR_FILENO);
		g_ex_states = WTERMSIG(status) + 128;
	}
	else
		g_ex_states = WEXITSTATUS(status);
}

static void	ms_close_heredoc_fd(t_clause *cl)
{
	t_clause	*tmp_cl;
	t_redir		*tmp_rd;

	tmp_cl = cl;
	while (tmp_cl)
	{
		tmp_rd = tmp_cl->redir;
		while (tmp_rd)
		{
			if (tmp_rd->heredoc_fd != 0)
			{
				if (close(tmp_rd->heredoc_fd) == -1)
					ms_perror("close");
			}
			tmp_rd = tmp_rd->next;
		}
		tmp_cl = tmp_cl->next;
	}
}

int	ms_executer(t_pipeline *pl, t_master *master, t_ex_state *state)
{
	if (pl == NULL)
		return (0);
	if (ms_heredoc(&pl->clause, state))
	{
		ms_close_heredoc_fd(pl->clause);
		return (1);
	}
	if (pl->clause->next != NULL)
		exec_pipe_command(pl, master, state);
	else
		g_ex_states = exec_simple_command(pl->clause, master, state);
	if ((pl->joint == TI_ANDAND && g_ex_states == 0) || \
		(pl->joint == TI_PIPEPIPE && g_ex_states != 0) || \
		pl->joint == TI_SEMICOLON)
	{
		if (!master->exited)
			ms_executer(pl->next, master, state);
	}
	ms_close_heredoc_fd(pl->clause);
	return (0);
}
