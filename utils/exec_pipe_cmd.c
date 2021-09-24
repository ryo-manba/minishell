/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:38 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/24 18:00:00 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

int	exec_check_piping(t_dpipe *dpipe, t_clause *clause)
{
	if (clause->next != NULL)
	{
		if (pipe(dpipe->new) == -1)
		{
			ms_perror("pipe");
			return (MS_EXEC_FAIL);
		}
	}
	return (MS_EXEC_SUCC);
}

// If it's a subshell, run it recursively.
void	exec_pipe_child(
	t_clause *clause, t_master *master, t_ex_state *es, t_dpipe *dpipe)
{
	t_stree	*expanded;

	ms_do_piping(clause, dpipe->new, dpipe->before);
	if (clause->stree && clause->stree->subshell)
	{
		exec_subshell(clause, master, es);
		exit(g_ex_states);
	}
	es->no_split = !!ms_is_special_builtin(clause->stree);
	expanded = ms_expand_stree(es, clause->stree);
	es->no_split = 0;
	if (!expanded && es->failed == 0 && !clause->redir)
		exit(0);
	if (!expanded && es->failed)
		exit(1);
	g_ex_states = exec_expand_redirect(es->master, clause);
	if (g_ex_states != MS_BLT_SUCC || !expanded)
		exit(g_ex_states);
	if (ms_is_builtin(expanded))
		exit(ms_exec_builtin(expanded, es->master));
	else
		exec_run_cmd_exit(es->master, expanded, master->var);
}

// Processing of command execution when a pipe is connected.
int	exec_pipe_command(t_pipeline *pl, t_master *master, t_ex_state *state)
{
	t_dpipe		dpipe;
	pid_t		pid;
	t_clause	*head;

	ft_memset(&dpipe, -1, sizeof(t_dpipe));
	head = pl->clause;
	while (head)
	{
		if (exec_check_piping(&dpipe, head) == MS_EXEC_FAIL)
			return (MS_EXEC_FAIL);
		pid = fork();
		if (pid < 0)
		{
			ms_perror("fork");
			return (1);
		}
		if (pid == 0)
			exec_pipe_child(head, master, state, &dpipe);
		else
			exec_pipe_parent(&dpipe);
		head = head->next;
	}
	exec_set_signal_wait(pid);
	return (g_ex_states);
}

void	exec_pipe_parent(t_dpipe *dpipe)
{
	ms_close_and_update_pipe(dpipe->new, dpipe->before);
}
