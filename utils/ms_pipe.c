#include "libft.h"
#include "../lexer/ms_lexer.h"
#include "ms_utils.h"

extern int debug_fd; // デバッグ用のファイルに書き込むため

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
// getenvはms_getenv作る
char	*ms_get_path(char *cmd)
{
	int		i;
	char	*path;
	char	**split_path;
	DIR		*dir;

	path = getenv("PATH");
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

// 子プロセスでの処理
// pipeをつなぐ->リダイレクト
// ビルトインなら実行してexit(), 通常コマンドならexecve()で実行する
void	ms_execute_child(t_clause *test, int pipe_fd[2], int before_pipe[2], char **envp)
{
//	char *new_cmd[2]; // ms_create_cmd()作る。
//	new_cmd[0] = test->cmd;
//	new_cmd[1] = NULL;

	ms_do_piping(test, pipe_fd, before_pipe);
	if (test->redir != NULL)
	{
		ms_redirect(test->redir->operand_left, test->operand_right, test->redir_op); // left = io_number, right = file_path
	}
//	if (test->is_builtin == true) // builtinならそのまま実行してexitする
//	{
//		exit(1); // do_builtin()
//	}
	else
	{
 		execve(ms_get_path(test->stree->token), new_cmd, envp); // builtin以外
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
void	ms_execute_command(t_clause *test, char **envp)
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
	new->next = NULL;
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

// テスト用
int	t_len(t_test *test)
{
	t_test *tmp;
	size_t	sz;

	sz = 0;
	tmp = test;
	while (tmp)
	{
		tmp = tmp->next;
		sz++;
	}
	return (sz);
}
