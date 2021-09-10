/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:09:03 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/10 17:06:38 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

// 最初のコマンド 標準出力をパイプの入り口に繋げる
int ms_first_pipe(int pipe_fd[2])
{
	if (close(pipe_fd[0]) == -1)
		return (MS_EXEC_FAIL);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		return (MS_EXEC_FAIL);
	if (close(pipe_fd[1]) == -1)
		return (MS_EXEC_FAIL);
	return (MS_EXEC_SUCC);
}

// 最後のコマンド 出力はそのままで入力だけ一つ前のpipeから受け取る
int ms_last_pipe(int before_pipe[2])
{
	if (close(before_pipe[1]) == -1)
		return (MS_EXEC_FAIL);
	if (dup2(before_pipe[0], STDIN_FILENO) == -1)
		return (MS_EXEC_FAIL);
	if (close(before_pipe[0]) == -1)
		return (MS_EXEC_FAIL);
	return (MS_EXEC_SUCC);
}

// 途中のコマンドなので上記の処理を両方やる
int ms_middle_pipe(int pipe_fd[2], int before_pipe[2])
{
	if (ms_last_pipe(before_pipe) == MS_EXEC_FAIL)
		return (MS_EXEC_FAIL);
	if (ms_first_pipe(pipe_fd) == MS_EXEC_FAIL)
		return (MS_EXEC_FAIL);
	return (MS_EXEC_SUCC);
}

// つなげ終わったパイプを閉じて一つ前のpipeを保持する
int	ms_close_and_update_pipe(int pipe_fd[2], int before_pipe[2])
{
	if (before_pipe[0] != -1 && before_pipe[1] != -1)
	{
		if (close(before_pipe[0]) == -1 || \
			close(before_pipe[1]) == -1)
			return (MS_EXEC_FAIL);
	}
	before_pipe[0] = pipe_fd[0];
	before_pipe[1] = pipe_fd[1];
	return (MS_EXEC_SUCC);
}

// 子プロセスでpipeをつなぐ
int	ms_do_piping(t_clause *clause, int pipe_fd[2], int before_pipe[2])
{
	if (before_pipe[0] == -1) // 最初
	{
		if (ms_first_pipe(pipe_fd) == 1)
			return (1);
	}
	else if (clause->next == NULL)
	{
		if (ms_last_pipe(before_pipe) == 1)
			return (1);
	}
	else
	{
		if (ms_middle_pipe(pipe_fd, before_pipe) == 1)
			return (1);
	}
	return (0);
}
