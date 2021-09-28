/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main_flow.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:54 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/28 22:44:32 by rmatsuka         ###   ########.fr       */
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

static void	ms_close_heredoc_fd(t_pipeline *pl)
{
	t_pipeline	*tmp_pl;
	t_clause	*tmp_cl;
	t_redir		*tmp_rd;

	tmp_pl = pl;
	while (tmp_pl)
	{
		tmp_cl = tmp_pl->clause;
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
		tmp_pl = tmp_pl->next;
	}
}

int	ms_pipeline_check(t_pipeline *pl)
{
	if ((pl->joint == TI_ANDAND && g_ex_states == 0) || \
		(pl->joint == TI_PIPEPIPE && g_ex_states != 0) || \
		pl->joint == TI_SEMICOLON)
		return (1);
	return (0);
}

int	ms_executer(t_pipeline *pl, t_master *master, t_ex_state *state)
{
	if (pl == NULL)
		return (0);
	if (ms_heredoc(pl, state))
	{
		ms_close_heredoc_fd(pl);
		return (1);
	}
	if (pl->clause->next != NULL)
		exec_pipe_command(pl, master, state);
	else
		g_ex_states = exec_simple_command(pl->clause, master, state);
	while (pl && !ms_pipeline_check(pl))
		pl = pl->next;
	if (pl && ms_pipeline_check(pl))
	{
		if (!master->exited)
			ms_executer(pl->next, master, state);
	}
	if (pl)
		ms_close_heredoc_fd(pl);
	return (0);
}
