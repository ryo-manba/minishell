/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:42 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/12 11:24:38 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

int	exec_check_path_state(t_ex_state *es, t_stree *expanded, char *path)
{
	if (es->last_exit_status == PERMISSION || es->last_exit_status == IS_A_DIR)
		exec_print_error_exit(es->last_exit_status, path);
	else if (path == NULL)
		exec_print_error_exit(es->last_exit_status, expanded->right->token);
	else
		return (MS_EXEC_SUCC);
	return (MS_EXEC_FAIL);
}

int	exec_ex_cmd(t_shellvar *var, t_stree *expanded)
{
	pid_t		pid;
	t_ex_state	es;

	ms_ex_init_state(&es, var, 0);
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
		exec_run_cmd_exit(expanded, var, &es);
	}
	else
	{
		if (signal(SIGINT, SIG_IGN) == SIG_ERR)
			ms_perror_exit("signal");
		exec_update_exitstatus(&es, pid);
		if (signal(SIGINT, ms_sigint_handler) == SIG_ERR)
			ms_perror_exit("signal");
	}
	return (es.last_exit_status);
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

	if (clause->redir)
	{
		if (exec_create_backup_fd(backup_fd) == MS_EXEC_FAIL)
			return (MS_EXEC_FAIL);
		if (exec_expand_redirect(clause, var) != MS_EXEC_SUCC)
		{
			exec_duplicate_backup_fd(backup_fd);
			return (MS_EXEC_FAIL);
		}
	}
	expanded = ms_expand_stree(es, clause->stree);
	if (!expanded)
		exit(1);
	if (ms_is_builtin(expanded) == 1)
		es->last_exit_status = ms_exec_builtin(var, expanded);
	else
		es->last_exit_status = exec_ex_cmd(var, expanded);
	if (clause->redir)
	{
		if (exec_duplicate_backup_fd(backup_fd) == 1)
			return (MS_EXEC_FAIL);
	}
	return (es->last_exit_status);
}
