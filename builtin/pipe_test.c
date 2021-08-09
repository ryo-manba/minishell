#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

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
		if (i != pipe_cnt)
		{
			pipe(pipe_fd);
		}
		pid = fork();
		if (pid == 0)
		{
			if (i == 0)
			{
				dup2(pipe_fd[1], 1); // 最初のコマンド標準出力をパイプの入り口に繋げる
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			else if (i == pipe_cnt) // 最後のコマンドなので標準出力をパイプの入り口へ繋げる
			{
				dup2(before_pipe[0], 0);
				close(before_pipe[0]);
				close(before_pipe[1]);
			}
			else					// 途中のコマンドなので上記の処理を両方やる
			{
				dup2(before_pipe[0], 0);
				dup2(pipe_fd[1], 1);
				close(before_pipe[0]);
				close(before_pipe[1]);
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			execvp(argv[pipe_locate[i] + 1], argv + pipe_locate[i] + 1);
		}
		else // つなげ終わったパイプを閉じる
		{
			close(before_pipe[0]);
			close(before_pipe[1]);
			before_pipe[0] = pipe_fd[0];
			before_pipe[1] = pipe_fd[1];
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
