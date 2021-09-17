/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:42 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/17 09:04:37 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

int	exec_subshell(t_clause *clause, t_shellvar *var, t_ex_state *es)
{
	ms_executer(clause->stree->subshell, var, es);
	return (g_ex_states);
}

int	exec_ex_cmd(t_master *master, t_shellvar *var, t_stree *expanded)
{
	pid_t		pid;
	t_ex_state	es;

	ms_ex_init_state(&es, master, var, 0);
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
int	exec_simple_command(t_clause *clause, t_shellvar *var, t_ex_state *es)
{
	int		backup_fd[3];
	t_stree	*expanded;

	if (clause->stree && clause->stree->subshell)
		return (exec_subshell(clause, var, es));
	expanded = ms_expand_stree(es, clause->stree);
	if (!expanded && es->failed == 0 && !clause->redir)
		return (MS_EXEC_SUCC);
	if (!expanded && es->failed)
		return (MS_EXEC_FAIL);
	if (exec_simple_redir(
			es->master, clause, var, backup_fd) == MS_EXEC_FAIL || !expanded)
	{
		exec_duplicate_backup_fd(backup_fd);
		return (MS_EXEC_FAIL);
	}
	if (ms_is_builtin(expanded))
		g_ex_states = ms_exec_builtin(var, expanded, es->master);
	else
		g_ex_states = exec_ex_cmd(es->master, var, expanded);
	if (clause->redir && exec_duplicate_backup_fd(backup_fd) == 1)
		return (MS_EXEC_FAIL);
	return (g_ex_states);
}

int	exec_simple_redir(t_master *master,
	t_clause *clause, t_shellvar *var, int backup_fd[3])
{
	if (clause->redir)
	{
		if (exec_create_backup_fd(backup_fd) == MS_EXEC_FAIL)
			return (MS_EXEC_FAIL);
		if (exec_expand_redirect(master, clause, var) != MS_EXEC_SUCC)
		{
			exec_duplicate_backup_fd(backup_fd);
			return (MS_EXEC_FAIL);
		}
	}
	return (MS_EXEC_SUCC);
}
