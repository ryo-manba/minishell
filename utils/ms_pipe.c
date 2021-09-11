/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:09:03 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/10 23:07:21 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

// つなげ終わったパイプを閉じて一つ前のpipeを保持する
void	ms_close_and_update_pipe(int pipe_fd[2], int before_pipe[2])
{
	if (before_pipe[0] != -1 && before_pipe[1] != -1)
	{
		if (close(before_pipe[0]) == -1 || \
			close(before_pipe[1]) == -1)
			ms_print_perror_exit("close");
	}
	before_pipe[0] = pipe_fd[0];
	before_pipe[1] = pipe_fd[1];
}

// 子プロセスでpipeをつなぐ
void	ms_do_piping(t_clause *clause, int pipe_fd[2], int before_pipe[2])
{
	if (before_pipe[0] == -1)
		ms_first_pipe(pipe_fd);
	else if (clause->next == NULL)
		ms_last_pipe(before_pipe);
	else
		ms_middle_pipe(pipe_fd, before_pipe);
}

// 最初のコマンド 標準出力をパイプの入り口に繋げる
void	ms_first_pipe(int pipe_fd[2])
{
	if (close(pipe_fd[0]) == -1)
		ms_print_perror_exit("close");
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		ms_print_perror_exit("dup2");
	if (close(pipe_fd[1]) == -1)
		ms_print_perror_exit("close");
}

// 最後のコマンド 出力はそのままで入力だけ一つ前のpipeから受け取る
void	ms_last_pipe(int before_pipe[2])
{
	if (close(before_pipe[1]) == -1)
		ms_print_perror_exit("close");
	if (dup2(before_pipe[0], STDIN_FILENO) == -1)
		ms_print_perror_exit("dup2");
	if (close(before_pipe[0]) == -1)
		ms_print_perror_exit("close");
}

// 途中のコマンドなので上記の処理を両方やる
void	ms_middle_pipe(int pipe_fd[2], int before_pipe[2])
{
	ms_last_pipe(before_pipe);
	ms_first_pipe(pipe_fd);
}
