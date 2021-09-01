#include "ms_utils.h"

// 環境変数を展開しながらリダイレクションを処理する
// エクスパンダーの部分修正する
void	ms_expand_and_redirect(t_clause *clause)
{
	while (clause->redir != NULL) // 逐次的にエキスパンドとリダイレクトを行う  echo hello > $VAR > b > c | cat
	{
		do_expander(clause->redir); // リダイレクションを展開する (echo a > $VAR　-> echo a > var)
		ms_redirect(clause->redir); // リダイレクションを処理する
		clause->redir = clause->redir->next;
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

void	ms_wait_child(int sz)
{
	while (sz--)
	{
		wait(NULL);
	}
}

void	ms_update_exitstatus(t_ex_state *state, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	state->last_exit_status = WEXITSTATUS(status);
}

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

// パイプラインを再帰的に処理する
int	ms_executer(t_pipeline *pl, t_shellvar *var, t_ex_state *state)
{
	t_dpipe *dpipe;
	int	status;

	if (pl == NULL)
		return (0);
	if (pl->clause->next != NULL) // パイプがある場合、終わるまでループ回す
	{
		ms_execute_pipe_command(pl, var, state);
	}
	else
	{
		state->last_exit_status = ms_simple_command(pl->clause, var);
	}
	if (pl->joint == TI_ANDAND && state->last_exit_status == 0 // && 前のコマンドが成功した場合
		|| pl->joint == TI_PIPEPIPE && state->last_exit_status == 1) // || 前のコマンドが失敗した場合
	{
		ms_executer(pl->next, var, state);
	}
	return (0);
}
