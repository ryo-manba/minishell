/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:42 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/15 04:11:32 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

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
		exec_run_cmd_exit(expanded, var);
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

// 親プロセスでリダイレクションをするとき用に、fd(0,1,2)のバックアップをとっておく。
int	exec_create_backup_fd(int backup_fd[3])
{
	backup_fd[0] = dup(STDIN_FILENO);
	backup_fd[1] = dup(STDOUT_FILENO);
	backup_fd[2] = dup(STDERR_FILENO);
	if (backup_fd[0] == -1 || backup_fd[1] == -1 || backup_fd[2] == -1)
		return (1);
	return (0);
}

// 親プロセスでリダイレクションをするとき用に、fd(0,1,2)のバックアップをとっておく。
int	exec_duplicate_backup_fd(int backup_fd[3])
{
	int	flag;

	flag = 0;
	if (dup2(backup_fd[0], STDIN_FILENO) == -1 || \
		dup2(backup_fd[1], STDOUT_FILENO) == -1 || \
		dup2(backup_fd[2], STDERR_FILENO) == -1)
		flag = 1;
	if (close(backup_fd[0]) == -1 || \
		close(backup_fd[1]) == -1 || \
		close(backup_fd[2]) == -1)
		flag = 1;
	return (flag);
}

// パイプなしのシンプルなコマンド
// ビルトインならそのまま実行する。
// 外部コマンドならforkして実行する。
// リダイレクトがある場合, バックアップをとって実行後に戻す
int	exec_simple_command(t_clause *clause, t_shellvar *var, t_ex_state *es)
{
	int		backup_fd[3];
	t_stree	*expanded;

	if (exec_simple_redir(es->master, clause, var, backup_fd) == MS_EXEC_FAIL)
		return (MS_EXEC_FAIL);
	expanded = ms_expand_stree(es, clause->stree);
	if (!expanded && es->failed == 0)
		return (MS_EXEC_SUCC);
	if (!expanded && es->failed == 1)
		return (MS_EXEC_FAIL);
	if (ms_is_builtin(expanded) == 1)
		g_ex_states = ms_exec_builtin(var, expanded);
	else
		g_ex_states = exec_ex_cmd(es->master, var, expanded);
	if (clause->redir)
	{
		if (exec_duplicate_backup_fd(backup_fd) == 1)
			return (MS_EXEC_FAIL);
	}
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
