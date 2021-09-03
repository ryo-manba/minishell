#include "ms_utils.h"

// 最初のコマンド 標準出力をパイプの入り口に繋げる
int ms_first_pipe(int pipe_fd[2])
{
	close(pipe_fd[0]);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		return (1);
	close(pipe_fd[1]);
	return (0);
}

// 最後のコマンド 出力はそのままで入力だけ一つ前のpipeから受け取る
int ms_last_pipe(int before_pipe[2])
{
	close(before_pipe[1]);
	if (dup2(before_pipe[0], STDIN_FILENO) == -1)
		return (1);
	close(before_pipe[0]);
}

// 途中のコマンドなので上記の処理を両方やる
int ms_middle_pipe(int pipe_fd[2], int before_pipe[2])
{
	if (ms_last_pipe(before_pipe) == 1)
		return (1);
	if (ms_first_pipe(pipe_fd) == 1)
		return (1);
	return (0);
}

// つなげ終わったパイプを閉じて一つ前のpipeを保持する
void ms_close_and_update_pipe(int pipe_fd[2], int before_pipe[2])
{
	close(before_pipe[0]);
	close(before_pipe[1]);
	before_pipe[0] = pipe_fd[0];
	before_pipe[1] = pipe_fd[1];
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
