#include "ms_utils.h"

// パイプラインを再帰的に処理する
// 修正する
int	ms_executer(t_pipeline *pl, t_shellvar *var, t_ex_state *state)
{
	if (pl == NULL)
		return (0);
	if (pl->clause->next != NULL) // パイプがある場合
		ms_do_piping(pl->clause);
	while (pl->clause->redir != NULL) // 逐次的にエキスパンドとリダイレクトを行う  echo hello > $VAR > b > c | cat
	{
		do_expander(pl->clause->redir); // リダイレクションを展開する (echo a > $VAR　-> echo a > var)
		ms_redirect(pl->clause->redir); // リダイレクションを処理する
		pl->clause->redir = pl->clause->redir->next;
	}
	if (pl->clause->stree->subshell != NULL) // subshellの場合、再帰的にエグゼキューターに渡す
	{
		ms_executer(pl->clause->stree->subshell, var, state);
	}
	ms_expander(pl->clause->stree); // echo $VAR -> echo var 展開する
	ms_execute_command(pl->clause->stree); // 実行する
	ms_update_exit_status();



	if (state->last_exit_status == 0) // &&
	{
		ms_executer(pl->next, var, state); // 再帰的にパイプラインを処理する
	}
	else if (state->last_exit_status == 1) // ||
	{
		ms_executer(pl->next, var, state); // 再帰的にパイプラインを処理する
	}
	return (0);
}


// パイプなしのシンプルなコマンド
// ビルトインならそのまま実行する。
// 外部コマンドならforkして実行する。
int	ms_simple_command(t_shellvar *var, t_clause *clause)
{
	pid_t	pid;
	int		status;

	ms_redirect(clause->redir);
	if (ms_is_builtin(clause->stree) == 1)
	{
		status = ms_execute_builtin(var, clause);
	}
	else
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
		}
		if (pid == 0)
		{
			errno = 0;
			execve(ms_get_path(clause->stree->token),
				ms_create_execute_command(clause->stree), NULL); // builtin以外
		}
		else
		{
			wait(NULL);
			if (errno != 0)
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(clause->stree->token, STDERR_FILENO);
				ft_putendl_fd(": command not found", STDERR_FILENO);
				status = errno;
			}
		}
	}
	return (status);
}
