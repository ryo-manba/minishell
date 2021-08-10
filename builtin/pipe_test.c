#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

// 最初のコマンド 標準出力をパイプの入り口に繋げる
void ms_first_pipe(int pipe_fd[2])
{
	dup2(pipe_fd[1], 1);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

// 最後のコマンド 出力はそのままで入力だけ一つ前のpipeから受け取る
void ms_last_pipe(int before_pipe[2])
{
	dup2(before_pipe[0], 0);
	close(before_pipe[0]);
	close(before_pipe[1]);
}

// 途中のコマンドなので上記の処理を両方やる
void ms_middle_pipe(int pipe_fd[2], int before_pipe[2])
{
	last_pipe(before_pipe);
	first_pipe(pipe_fd);
}

// つなげ終わったパイプを閉じて一つ前のpipeを保持する
void ms_close_and_update_pipe(int pipe_fd[2], int before_pipe[2])
{
	close(before_pipe[0]);
	close(before_pipe[1]);
	before_pipe[0] = pipe_fd[0];
	before_pipe[1] = pipe_fd[1];
}

int main()
{
	int pipe_fd[2];
	int before_pipe[2] = {-1, -1};
	int	pipe_cnt = 0;
	int pipe_locate[10];
	int i;
	pid_t	pid;
	char *argv[] = {"ls", "|", "cat", "|", "cat", "|", "cat", "|",  "wc", NULL};
	pipe_locate[0] = -1; // 先頭のコマンド用
	i = 0;
	while (argv[i])
	{
		if (strcmp(argv[i], "|") == 0)
		{
			pipe_cnt++;
			pipe_locate[pipe_cnt] = i;
			argv[i] = NULL;
		}
		i++;
	}
	i = 0;
	while (i < pipe_cnt + 1) // 実行するのはpipeの数足す1個
	{
		if (i != pipe_cnt) // 最後以外pipeを作る
		{
			pipe(pipe_fd);
		}
		pid = fork();
		if (pid == 0)
		{
			if (i == 0)
			{
				ms_first_pipe(pipe_fd);
			}
			else if (i == pipe_cnt)
			{
				ms_last_pipe(before_pipe);
			}
			else
			{
				ms_middle_pipe(pipe_fd, before_pipe);
			}
			execvp(argv[pipe_locate[i] + 1], argv + pipe_locate[i] + 1);
		}
		else
		{
			ms_close_and_update_pipe(pipe_fd, before_pipe); // 親でpipeを閉じる
		}
		i++;
	}
	i = 0;
	while (i < pipe_cnt)
	{
		wait(NULL);
		i++;
	}
	return (0);
}
