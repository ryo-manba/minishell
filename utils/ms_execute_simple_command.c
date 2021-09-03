#include "ms_utils.h"

void	ms_print_exec_error(t_clause *clause)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(clause->stree->token, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
}

// 親プロセスでリダイレクションをするとき用に、fd(0,1,2)のバックアップをとっておく。
int	ms_duplicate_backup_fd(int backup_fd[3])
{
	int	flag;

	flag = 0;
	if (dup2(backup_fd[0], STDIN_FILENO) == -1
		|| dup2(backup_fd[1], STDOUT_FILENO) == -1
		|| dup2(backup_fd[2], STDERR_FILENO) == -1)
			flag = 1;
	close(backup_fd[0]);
	close(backup_fd[1]);
	close(backup_fd[2]);
	return (flag);
}

// 親プロセスでリダイレクションをするとき用に、fd(0,1,2)のバックアップをとっておく。
int	ms_create_backup_fd(int backup_fd[3])
{
	backup_fd[0] = dup(STDIN_FILENO);
	backup_fd[1] = dup(STDOUT_FILENO);
	backup_fd[2] = dup(STDERR_FILENO);
	if (backup_fd[0] == -1 || backup_fd[1] == -1 || backup_fd[2] == -1)
		return (1);
	return (0);
}

int	ms_execute_child(t_clause *clause)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
	}
	if (pid == 0)
	{
		errno = 0;
		execve(ms_get_path(clause->stree->token),
			ms_create_execute_command(clause->stree), NULL);
	}
	else
	{
		wait(NULL);
		if (errno != 0)
		{
			ms_print_exec_error(clause);
			return (errno);
		}
	}
	return (0);
}

// パイプなしのシンプルなコマンド
// ビルトインならそのまま実行する。
// 外部コマンドならforkして実行する。
int	ms_simple_command(t_clause *clause, t_shellvar *var)
{
	int		status;
	int		backup_fd[3];

	if (clause->redir)// リダイレクトがある場合, バックアップをとって実行後に戻す
	{
		if (ms_create_backup_fd(backup_fd) == 1)
			return (1);
		ms_expand_and_redirect(clause); // 変数展開とリダイレクト処理
	}
	if (ms_is_builtin(clause->stree) == 1)
	{
		status = ms_execute_builtin(var, clause);
	}
	else
	{
		ms_execute_child(clause); // ビルトイン以外なら以外なら子プロセスで実行する
	}
	if (clause->redir)
	{
		if (ms_duplicate_backup_fd(backup_fd) == 1) // リダイレクトしていたらfd(0,1,2)を元に戻す
			return (1);
	}
	return (status);
}
