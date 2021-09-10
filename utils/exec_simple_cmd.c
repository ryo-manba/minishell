/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:42 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/10 17:34:33 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

// 親プロセスでリダイレクションをするとき用に、fd(0,1,2)のバックアップをとっておく。
int	exec_duplicate_backup_fd(int backup_fd[3])
{
	int	flag;

	flag = 0;
	if (dup2(backup_fd[0], STDIN_FILENO) == -1 || \
		dup2(backup_fd[1], STDOUT_FILENO) == -1 || \
		dup2(backup_fd[2], STDERR_FILENO) == -1)
			flag = 1;
	if (exec_close_backup_fd(backup_fd) == 1)
		flag = 1;
	return (flag);
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

int	exec_close_backup_fd(int backup_fd[3])
{
	if (close(backup_fd[0]) == -1 || \
		close(backup_fd[1] == -1) || \
		close(backup_fd[2]) == -1)
		return (1);
	return (0);
}

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

int	exec_child(t_shellvar *var, t_stree *expanded)
{
	pid_t		pid;
	t_ex_state	es;
	char		*path;

	ms_ex_init_state(&es, var, 0);
	pid = fork();
	if (pid < -1)
	{
		ms_print_perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		path = exec_get_path(expanded->token, var, &es);
		if (exec_check_path_state(&es, expanded, path) == MS_EXEC_FAIL)
			exit(NO_SUCH_FILE);
		execve(path, exec_create_command(expanded), NULL);
		exit(CMD_NOT_FOUND);
	}
	else
	{
		ms_update_exitstatus(&es, pid);
		if (es.last_exit_status == CMD_NOT_FOUND)
		{
			exec_print_error(expanded->token);
			return (MS_EXEC_FAIL);
		}
	}
	return (MS_EXEC_SUCC);
}

// パイプなしのシンプルなコマンド
// ビルトインならそのまま実行する。
// 外部コマンドならforkして実行する。
int	exec_simple_command(t_clause *clause, t_shellvar *var, t_ex_state *state)
{
	int		status;
	int		backup_fd[3];
	t_stree	*expanded;

	if (clause->redir)// リダイレクトがある場合, バックアップをとって実行後に戻す
	{
		if (exec_create_backup_fd(backup_fd) == 1)
			return (1);
		if (exec_expand_redirect(clause, var) == 1) // 変数展開とリダイレクト処理
			return (1);
	}
	expanded = ms_expand_stree(state, clause->stree); // echo $VAR -> echo var 展開する
	if (!expanded)
		exit(1);
	print_stree(expanded, 0);
	printf("\n");
	if (ms_is_builtin(expanded) == 1)
		status = ms_exec_builtin(var, expanded);
	else
		status = exec_child(var, expanded); // ビルトイン以外なら以外なら子プロセスで実行する
	if (clause->redir)
	{
		if (exec_duplicate_backup_fd(backup_fd) == 1) // リダイレクトしていたらfd(0,1,2)を元に戻す
			return (1);
	}
	return (status);
}
