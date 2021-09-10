/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:38 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/10 18:08:18 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"
// エラーチェックとパイプを閉じる
int	exec_pipe_parent(t_pipeline *pl, t_dpipe *dpipe)
{
	(void)pl;
//	if (errno != 0)
//	{
//		exec_print_error(pl->clause->stree->token);
//		return (MS_EXEC_FAIL);
//	}
	if (ms_close_and_update_pipe(dpipe->new, dpipe->before) == MS_BLT_FAIL) // 親でpipeを閉じる
		return (MS_EXEC_FAIL);
	return (MS_EXEC_SUCC);
}

// コマンド実行の核
// エクスパンダーの部分修正する
void	exec_pipe_child(t_pipeline *pl, t_shellvar *var, t_ex_state *state, t_dpipe *dpipe)
{
	char 	*path;
	t_stree	*expanded;

	ms_do_piping(pl->clause, dpipe->new, dpipe->before); // パイプをつなげる
	exec_expand_redirect(pl->clause, var); 		// 変数展開してリダイレクト
	if (pl->clause->stree->subshell != NULL) 	// subshellの場合、再帰的にエグゼキューターに渡す
	{
		ms_executer(pl->clause->stree->subshell, var, state);
	}
	expanded = ms_expand_stree(state, pl->clause->stree); // echo $VAR -> echo var 展開する
	if (!expanded)
		exit(1);
	if (ms_is_builtin(expanded) == 1) // builtinならそのまま実行してexitする
	{
		exit(ms_exec_builtin(var, expanded));
	}
	else
	{
		path = exec_get_path(expanded->token,var, state);
		if (exec_check_path_state(state, expanded, path) == MS_EXEC_FAIL)
			exit(NO_SUCH_FILE);
		execve(path,exec_create_command(expanded), NULL);
		_exit(1);
		exit(CMD_NOT_FOUND);
	}
}

// forkした分の子プロセスを回収する
void	exec_wait_child(int sz)
{
	while (sz--)
	{
		wait(NULL);
	}
}

// パイプが繋がっていた場合のコマンド実行の処理
int	exec_pipe_command(t_pipeline *pl, t_shellvar *var, t_ex_state *state)
{
	t_dpipe dpipe;
	pid_t	pid;
	int		child_sum;

	child_sum = 0;
	ft_memset(&dpipe, -1, sizeof(t_dpipe));
	while (pl->clause) // すべてのコマンドを実行していく
	{
		if (pl->clause->next != NULL)
		{
			if (pipe(dpipe.new) == -1)
			{
				ms_print_perror("pipe");
				return (1);
			}
		}
		pid = fork();
		if (pid < 0)
		{
			ms_print_perror("fork");
			return (1);
		}
		if (pid == 0)
			exec_pipe_child(pl, var, state, &dpipe);
		else
			exec_pipe_parent(pl, &dpipe);
		pl->clause = pl->clause->next;
		child_sum += 1;
	}
	ms_update_exitstatus(state, pid); // 最後のコマンドのpidからステータスを取る
	exec_wait_child(child_sum);
	return (state->last_exit_status);
}
