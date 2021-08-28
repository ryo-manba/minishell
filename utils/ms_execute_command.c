#include "ms_utils.h"

char	**ms_create_execute_command()
{
	char **new_cmd;
//	char *new_cmd[2]; // ms_create_cmd()作る。
//	new_cmd[0] = test->cmd;
//	new_cmd[1] = NULL;

	return (new_cmd);
}

// 子プロセスでの処理
// pipeをつなぐ->リダイレクト
// ビルトインなら実行してexit(), 通常コマンドならexecve()で実行する
void	ms_execute_child(t_clause *test, int pipe_fd[2], int before_pipe[2], char **envp)
{
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
