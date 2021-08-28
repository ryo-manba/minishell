#include "ms_utils.h"

// 最初のコマンド 標準出力をパイプの入り口に繋げる
void ms_first_pipe(int pipe_fd[2])
{
	close(pipe_fd[0]);
	dup2(pipe_fd[1], 1);
	close(pipe_fd[1]);
}

// 最後のコマンド 出力はそのままで入力だけ一つ前のpipeから受け取る
void ms_last_pipe(int before_pipe[2])
{
	close(before_pipe[1]);
	dup2(before_pipe[0], 0);
	close(before_pipe[0]);
}

// 途中のコマンドなので上記の処理を両方やる
void ms_middle_pipe(int pipe_fd[2], int before_pipe[2])
{
	ms_last_pipe(before_pipe);
	ms_first_pipe(pipe_fd);
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
int	ms_do_piping(t_clause *test, int pipe_fd[2], int before_pipe[2])
{
	if (before_pipe[0] == -1) // 最初
	{
		ms_first_pipe(pipe_fd);
	}
	else if (test->next == NULL) // 次のpipelineがない場合
	{
		ms_last_pipe(before_pipe);
	}
	else
	{
		ms_middle_pipe(pipe_fd, before_pipe);
	}
	return (0);
}
