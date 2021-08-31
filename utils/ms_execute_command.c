#include "ms_utils.h"

// コマンドの引数を数える
// $ "ls -l" = 2
size_t	ms_get_cmd_size(t_stree *tree)
{
	t_stree *tmp;
	size_t	sz;

	tmp = tree;
	sz = 0;
	while (tmp != NULL)
	{
		tmp = tmp->right;
		sz++;
	}
	return (sz);
}

// execveに送る場合
// {"ls", "-l", "NULL"}の状態にする。
char	**ms_create_execute_command(t_stree *tree)
{
	char **new_cmd;
	size_t	sz;
	size_t	i;
	t_stree	*tmp;

	sz = ms_get_cmd_size(tree);
	new_cmd = (char **)malloc(sizeof(char *) * sz + 1);
	tmp = tree;
	i = 0;
	while (tmp != NULL)
	{
		new_cmd[i] = ft_strdup(tmp->token);
		tmp = tmp->right;
		i++;
	}
	new_cmd[i] = NULL;
	return (new_cmd);
}

// 子プロセスでの処理
// pipeをつなぐ->リダイレクト
// ビルトインなら実行してexit(), 通常コマンドならexecve()で実行する
void	ms_execute_child(t_clause *clause, int pipe_fd[2], int before_pipe[2], char **envp)
{
	if (ms_is_builtin(clause->stree) == 1) // builtinならそのまま実行してexitする
	{
		exit(ms_exec_builtin(clause->stree));
	}
	else
	{
		execve(ms_get_path(clause->stree->token),
			ms_create_execute_command(clause->stree), envp); // builtin以外
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
// clauseはシンプルコマンド
// || && ; ではない
void	ms_execute_command(t_clause *clause, t_shellvar *env)
{
	static int	before_pipe[2] = {-1, -1};
	int	pipe_fd[2];
	pid_t pid;
	if (clause->next != NULL) // 最後以外pipeを作る
	{
		pipe(pipe_fd);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		ms_execute_child(clause, pipe_fd, before_pipe, env);
	}
	else
	{
		ms_close_and_update_pipe(pipe_fd, before_pipe); // 親でpipeを閉じる
	}
}
