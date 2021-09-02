#include "ms_utils.h"

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
int	ms_execute_pipe_child(t_pipeline *pl, t_shellvar *var, t_ex_state *state, t_dpipe *dpipe)
{
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
		execve(ms_get_path(pl->clause->stree->token),
			ms_create_execute_command(pl->clause->stree), NULL); // builtin以外
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
