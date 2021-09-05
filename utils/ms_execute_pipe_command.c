#include "ms_utils.h"

void	print_error_exit(int ex_status, char *path)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (ex_status == IS_A_DIR || ex_status == PERMISSION)
	{
		if (ex_status == IS_A_DIR)
			ft_putendl_fd("is a directory", STDERR_FILENO);
		if (ex_status == PERMISSION)
			ft_putendl_fd("Permission denied", STDERR_FILENO);
		exit(126);
	}
	else
	{
		ft_putendl_fd("command not found", STDERR_FILENO);
		exit(127);
	}
}

// エラーチェックとパイプを閉じる
int	ms_execute_pipe_parent(t_pipeline *pl, t_ex_state *state, t_dpipe *dpipe ,pid_t pid)
{
	if (errno != 0)
	{
		ms_print_exec_error(pl->clause);
	}
	ms_close_and_update_pipe(dpipe->new, dpipe->before); // 親でpipeを閉じる
}

// コマンド実行の核
// エクスパンダーの部分修正する
void	ms_execute_pipe_child(t_pipeline *pl, t_shellvar *var, t_ex_state *state, t_dpipe *dpipe)
{
	char *path;

	ms_do_piping(pl->clause, dpipe->new, dpipe->before); // パイプをつなげる
	ms_expand_and_redirect(pl->clause); 		// 変数展開してリダイレクト
	if (pl->clause->stree->subshell != NULL) 	// subshellの場合、再帰的にエグゼキューターに渡す
	{
		ms_executer(pl->clause->stree->subshell, var, state);
	}
	ms_expander(pl->clause->stree); // echo $VAR -> echo var 展開する
	if (ms_is_builtin(pl->clause->stree) == 1) // builtinならそのまま実行してexitする
	{
		exit(ms_exec_builtin(pl->clause->stree));
	}
	else
	{
		errno = 0;
		path = ms_get_path(pl->clause->stree->right->token,var, state);
		if (state->last_exit_status == PERMISSION || state->last_exit_status == IS_A_DIR)
		{
			ms_print_error_exit(state->last_exit_status, path);
		}
		else if (path == NULL)
			ms_print_error_exit(state->last_exit_status, pl->clause->stree->right->token);
		else
		{
			execve(path,ms_create_execute_command(pl->clause->stree), NULL);
		}
		exit(errno);
	}
}

// forkした分の子プロセスを回収する
void	ms_wait_child(int sz)
{
	while (sz--)
	{
		wait(NULL);
	}
}

// パイプが繋がっていた場合のコマンド実行の処理
int	ms_execute_pipe_command(t_pipeline *pl, t_shellvar *var, t_ex_state *state)
{
	t_dpipe *dpipe;
	pid_t	pid;
	int		sz;

	sz = 0;
	while (pl->clause != NULL) // すべてのコマンドを実行していく
	{
		if (pl->clause->next != NULL)
		{
			pipe(dpipe->new);
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
		}
		if (pid == 0)
		{
			ms_execute_pipe_child(pl, var, state, dpipe);
		}
		else
		{
			ms_execute_pipe_parent(pl, state, dpipe, pid);
		}
		pl->clause = pl->clause->next;
		sz++;
	}
	ms_update_exitstatus(state, pid);
	ms_wait_child(sz);
}
