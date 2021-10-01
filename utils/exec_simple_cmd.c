/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:42 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/10/01 12:00:35 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

int	exec_subshell(t_clause *clause,
	t_master *master, t_ex_state *es, int backup_fd[3])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		ms_perror("fork");
		return (1);
	}
	else if (pid == 0)
	{
		ms_executer(clause->stree->subshell, master, es);
		exit(g_ex_states);
	}
	else
		exec_update_exitstatus(pid);
	if (backup_fd)
		exec_duplicate_backup_fd(backup_fd);
	return (g_ex_states);
}

static int	exec_ex_cmd(t_master *master, t_shellvar *var, t_stree *expanded)
{
	pid_t		pid;

	pid = fork();
	if (pid < -1)
	{
		ms_perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
			ms_perror_exit("signal");
		exec_run_cmd_exit(master, expanded, var);
	}
	else
	{
		if (signal(SIGINT, SIG_IGN) == SIG_ERR)
			ms_perror_exit("signal");
		exec_update_exitstatus(pid);
		if (signal(SIGINT, ms_sigint_handler) == SIG_ERR)
			ms_perror_exit("signal");
	}
	return (g_ex_states);
}

static int	exec_simple_redir(t_master *master,
	t_clause *clause, int backup_fd[3])
{
	int	rv;

	if (clause->redir)
	{
		rv = exec_expand_redirect(master, clause, backup_fd);
		if (rv)
		{
			exec_duplicate_backup_fd(backup_fd);
			return (rv);
		}
	}
	return (MS_EXEC_SUCC);
}

static int	exec_close_fd(t_redir *rd)
{
	t_redir	*tmp;
	int		io_number;

	if (rd == NULL)
		return (1);
	tmp = rd;
	while (tmp)
	{
		if (tmp->operand_left)
		{
			io_number = ft_atoi(tmp->operand_left->token);
			if (!(0 <= io_number && io_number <= 2))
				close(io_number);
		}
		tmp = tmp->next;
	}
	return (1);
}

// If built-in, run as is.
// If it's an external command, fork it and run it.
// If there is a redirect, make a backup and revert after execution.
int	exec_simple_command(t_clause *clause, t_master *master, t_ex_state *es)
{
	int		backup_fd[3];
	t_stree	*expanded;

	exec_create_backup_fd(backup_fd);
	es->no_split = !!ms_is_special_builtin(clause->stree);
	expanded = ms_expand_stree(es, clause->stree, 0);
	es->no_split = 0;
	if (!expanded && es->failed == 0 && !clause->redir)
		return (MS_EXEC_SUCC);
	if (!expanded && es->failed)
		return (MS_EXEC_FAIL);
	if (exec_simple_redir(es->master, clause, backup_fd))
	{
		exec_duplicate_backup_fd(backup_fd);
		return (exec_out(MS_EXEC_FAIL, expanded));
	}
	if (clause->stree && clause->stree->subshell)
		return (exec_subshell(clause, master, es, backup_fd));
	if (ms_is_builtin(expanded))
		g_ex_states = ms_exec_builtin(expanded, es->master);
	else
		g_ex_states = exec_ex_cmd(es->master, master->var, expanded);
	if (exec_close_fd(clause->redir) && exec_duplicate_backup_fd(backup_fd))
		return (exec_out(MS_EXEC_FAIL, expanded));
	return (exec_out(g_ex_states, expanded));
}
