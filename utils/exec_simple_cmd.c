/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:42 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/20 15:22:25 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

int	exec_subshell(t_clause *clause, t_master *master, t_ex_state *es)
{
	ms_executer(clause->stree->subshell, master, es);
	return (g_ex_states);
}

int	exec_ex_cmd(t_master *master, t_shellvar *var, t_stree *expanded)
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

// パイプなしのシンプルなコマンド
// ビルトインならそのまま実行する。
// 外部コマンドならforkして実行する。
// リダイレクトがある場合, バックアップをとって実行後に戻す
int	exec_simple_command(t_clause *clause, t_master *master, t_ex_state *es)
{
	int		backup_fd[3];
	t_stree	*expanded;

	if (clause->stree && clause->stree->subshell)
		return (exec_subshell(clause, master, es));
	es->no_split = !!ms_is_special_builtin(clause->stree);
	expanded = ms_expand_stree(es, clause->stree);
	es->no_split = 0;
	if (!expanded && es->failed == 0 && !clause->redir)
		return (MS_EXEC_SUCC);
	if (!expanded && es->failed)
		return (MS_EXEC_FAIL);
	if (exec_simple_redir(
			es->master, clause, backup_fd) == MS_EXEC_FAIL)
	{
		exec_duplicate_backup_fd(backup_fd);
		return (MS_EXEC_FAIL);
	}
	if (ms_is_builtin(expanded))
		g_ex_states = ms_exec_builtin(expanded, es->master);
	else
		g_ex_states = exec_ex_cmd(es->master, master->var, expanded);
	if (clause->redir && exec_duplicate_backup_fd(backup_fd) == 1)
		return (MS_EXEC_FAIL);
	return (g_ex_states);
}

int	exec_simple_redir(t_master *master,
	t_clause *clause, int backup_fd[3])
{
	int	rv;

	if (clause->redir)
	{
		rv = exec_create_backup_fd(backup_fd);
		if (rv)
			return (MS_EXEC_FAIL);
		rv = exec_expand_redirect(master, clause);
		if (rv)
		{
			exec_duplicate_backup_fd(backup_fd);
			return (rv);
		}
	}
	return (MS_EXEC_SUCC);
}
