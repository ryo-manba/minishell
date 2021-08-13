#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <dirent.h>
#include <fcntl.h>
#include "libft.h"
#include "../lexer/ms_lexer.h"
#include "ms_utils.h"

int debug_fd;
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

//　実行パスをディレクトリから探す
char *ms_search_execution_path(DIR *dir, char *cmd, char *path)
{
	struct dirent *dirent;
	char *tmp;
	char *execution_path;

	while (1) 	// opendirに失敗したままreaddirするとセグフォする
	{
		dirent = readdir(dir);
		if (dirent == NULL)
			break ;
		if (ft_strcmp(dirent->d_name, cmd) == 0)
		{
			closedir(dir);
			tmp = ft_strjoin(path, "/");
			execution_path = ft_strjoin(tmp, cmd);
			free(tmp);
			return (execution_path);
		}
	}
	return (NULL);
}

// 実行パスを返す
char	*ms_get_path(char *cmd)
{
	int		i;
	char	*path;
	char	**split_path;
	DIR		*dir;

	path = getenv("PATH"); // getenvで取得した場合リークにはならない
	split_path = ft_split(path, ':');
	i = -1;
	while (split_path[++i])
	{
		dir = opendir(split_path[i]); // 権限がない場合もopendirに失敗する
		if (dir == NULL)
			continue ;
		path = ms_search_execution_path(dir, cmd, split_path[i]);
		if (path != NULL)
			return (path);
		closedir(dir);
	}
	return (NULL);
}

// 子プロセスでpipeをつなぐ
int	ms_do_piping(t_test *test, int pipe_fd[2], int before_pipe[2])
{
	if (before_pipe[0] == -1) // 最初
	{
//		printf("first_pipe %s\n", test->cmd);
		ms_first_pipe(pipe_fd);
	}
	else if (test->next == NULL) // 次のpipelineがない場合
	{
//		printf("last_pipe %s\n", test->cmd);
		ms_last_pipe(before_pipe);
	}
	else
	{
//		printf("middle_pipe %s\n", test->cmd);
		ms_middle_pipe(pipe_fd, before_pipe);
	}
	return (0);
}

// 子プロセスでの処理
// pipeをつなぐ->リダイレクト
// ビルトインなら実行してexit(), 通常コマンドならexecve()で実行する
void	ms_execute_child(t_test *test, int pipe_fd[2], int before_pipe[2], char **envp)
{
	char *new_cmd[2];
	new_cmd[0] = test->cmd;
	new_cmd[1] = NULL;

	ms_do_piping(test, pipe_fd, before_pipe);
	if (test->has_redirect && ms_redirect(test->io_number, test->file_path, test->detail_type) == 0) // redirect(io_number or file_path)
	{
		strerror(errno);
		exit(1);
	}
	if (test->is_builtin == true) // builtinならそのまま実行してexitする
	{
		exit(1); // do_builtin()
	}
	else
	{
//		dprintf(debug_fd, "%s, %s\n", test->cmd, ms_get_path(test->cmd));
 		execve(ms_get_path(test->cmd), new_cmd, envp); // builtin以外
	}
}

/**
 * Execute a simple command that is hopefully defined in a disk file somewhere.
 *
 * 1) fork ()
 * 2) connect pipes
 * 3) look up the command
 * 4) do redirections
 * 5) execve ()
 * 6) If the execve failed, see if the file has executable mode set.
 */
// bashのコメントに沿ってに沿ってコマンドを実行する
void	ms_execute_command(t_test *test, char **envp)
{
	static int	before_pipe[2] = {-1, -1};
	int	pipe_fd[2];
	pid_t pid;

	if (test->next != NULL) // 最後以外pipeを作る
	{
		pipe(pipe_fd);
	}
	if ((pid = fork()) < 0)
	{
		strerror(errno);
		exit(1);
	}
	if (pid == 0)
	{
		ms_execute_child(test, pipe_fd, before_pipe, envp);
	}
	else
	{
		ms_close_and_update_pipe(pipe_fd, before_pipe); // 親でpipeを閉じる
	}
}

// テスト用
void	t_pushback(t_test *test, int io_number, int detail_type, char *path, char *cmd)
{
	t_test *new;
	t_test *tmp;
	static int i = 0;

	new = calloc(1, sizeof(t_test));
	new->io_number = io_number;
	new->detail_type = detail_type;
	new->file_path = path;
	new->cmd = cmd;
	new->is_builtin = false;
	if (io_number == -1)
		new->has_redirect = false;
	else
		new->has_redirect = true;
	tmp = test;
	while (tmp->next)
	{
		tmp = tmp->next;
	}
	tmp->next = new;
}

int main(int argc, char **argv, char **envp)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	t_test *test;
	t_test *tmp;

	debug_fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	test = calloc(1, sizeof(t_test));
	// (構造体, io_number, type, path, command)
	t_pushback(test, -1, -1, NULL, "ls");
	t_pushback(test, -1, -1, NULL, "cat");
	t_pushback(test, -1, -1, NULL, "cat");
	t_pushback(test, -1, -1, NULL, "cat");
	t_pushback(test, -1, -1, NULL, "wc");
	test = test->next;
	tmp = test;
	// t_pushback(test, -1, REDIRECT_OUTPUT, NULL, "ls");
	// t_pushback(test, -1, REDIRECT_INPUT, NULL, "cat");
	// t_pushback(test, -1, REDIRECT_APPEND, NULL, "cat");
	while (test)
	{
		ms_execute_command(test, envp);
		test = test->next;
	}
	while (tmp)
	{
		wait(NULL);
		tmp = tmp->next;
	}
	return (0);
}
