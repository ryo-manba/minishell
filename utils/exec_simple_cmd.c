/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:42 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/10 02:56:33 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

// 親プロセスでリダイレクションをするとき用に、fd(0,1,2)のバックアップをとっておく。
int	exec_duplicate_backup_fd(int backup_fd[3])
{
	int	flag;

	flag = 0;
	if (dup2(backup_fd[0], STDIN_FILENO) == -1
		|| dup2(backup_fd[1], STDOUT_FILENO) == -1
		|| dup2(backup_fd[2], STDERR_FILENO) == -1)
			flag = 1;
	close(backup_fd[0]);
	close(backup_fd[1]);
	close(backup_fd[2]);
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

int	exec_child(t_clause *clause, t_shellvar *var)
{
	pid_t	pid;
	t_ex_state	es;

	ms_ex_init_state(&es, var, 0);
	pid = fork();
	if (pid < -1)
	{
		ms_print_perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		errno = 0;
		execve(exec_get_path(clause->stree->token, var, &es),
			exec_create_command(clause->stree), NULL);
	}
	else
	{
		wait(NULL);
		if (errno != 0)
		{
			exec_print_error(clause);
			return (errno);
		}
	}
	return (0);
}

// パイプなしのシンプルなコマンド
// ビルトインならそのまま実行する。
// 外部コマンドならforkして実行する。
int	exec_simple_command(t_clause *clause, t_shellvar *var)
{
	int		status;
	int		backup_fd[3];

	if (clause->redir)// リダイレクトがある場合, バックアップをとって実行後に戻す
	{
		if (exec_create_backup_fd(backup_fd) == 1)
			return (1);
		if (exec_expand_redirect(clause, var) == 1) // 変数展開とリダイレクト処理
			return (1);
	}
	if (ms_is_builtin(clause->stree) == 1)
	{
		status = ms_exec_builtin(var, clause->stree);
	}
	else
	{
		exec_child(clause, var); // ビルトイン以外なら以外なら子プロセスで実行する
	}
	if (clause->redir)
	{
		if (exec_duplicate_backup_fd(backup_fd) == 1) // リダイレクトしていたらfd(0,1,2)を元に戻す
			return (1);
	}
	return (status);
}
