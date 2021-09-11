/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:38 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/11 20:52:53 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

int	exec_check_piping(t_dpipe *dpipe, t_clause *clause)
{
	if (clause->next != NULL)
	{
		if (pipe(dpipe->new) == -1)
		{
			ms_perror("pipe");
			return (MS_EXEC_FAIL);
		}
	}
	return (MS_EXEC_SUCC);
}

// subshellの場合、再帰的にエグゼキューターに渡す
// builtinの場合は、実行してexitする
// TODO: subshellの処理
void	exec_pipe_child(
	t_pipeline *pl, t_shellvar *var, t_ex_state *es, t_dpipe *dpipe)
{
	t_stree	*expanded;

	ms_do_piping(pl->clause, dpipe->new, dpipe->before);
	es->last_exit_status = exec_expand_redirect(pl->clause, var);
	if (es->last_exit_status != MS_BLT_SUCC)
		exit(es->last_exit_status);
	if (pl->clause->stree->subshell != NULL)
		ms_executer(pl->clause->stree->subshell, var, es);
	expanded = ms_expand_stree(es, pl->clause->stree);
	if (!expanded)
		exit(1);
	if (ms_is_builtin(expanded))
		exit(ms_exec_builtin(var, expanded));
	else
		exec_run_cmd_exit(expanded, var, es);
}

// パイプが繋がっていた場合のコマンド実行の処理
// 最後のコマンドのpidからステータスを取る
int	exec_pipe_command(t_pipeline *pl, t_shellvar *var, t_ex_state *state)
{
	t_dpipe	dpipe;
	pid_t	pid;

	ft_memset(&dpipe, -1, sizeof(t_dpipe));
	while (pl->clause)
	{
		if (exec_check_piping(&dpipe, pl->clause) == MS_EXEC_FAIL)
			return (MS_EXEC_FAIL);
		pid = fork();
		if (pid < 0)
		{
			ms_perror("fork");
			return (1);
		}
		if (pid == 0)
			exec_pipe_child(pl, var, state, &dpipe);
		else
			exec_pipe_parent(&dpipe);
		pl->clause = pl->clause->next;
	}
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		ms_perror_exit("signal");
	exec_update_exitstatus(state, pid);
	while (wait(NULL) > 0)
		;
	if (signal(SIGINT, ms_sigint_handler) == SIG_ERR)
		ms_perror_exit("signal");
	return (state->last_exit_status);
}

// 親でpipeを閉じる
void	exec_pipe_parent(t_dpipe *dpipe)
{
	ms_close_and_update_pipe(dpipe->new, dpipe->before);
}

// '/' が含まれてる場合はそのまま実行する
// ない場合はPATHから探す
void	exec_run_cmd_exit(t_stree *expanded, t_shellvar *var, t_ex_state *state)
{
	char	*path;

	if (ft_strchr_i(expanded->token, '/') != -1)
		path = expanded->token;
	else
		path = exec_get_path(expanded->token, var, state);
	if (exec_check_path_state(state, expanded, path) == MS_EXEC_FAIL)
		exec_print_error_exit(NO_SUCH_FILE, NULL);
	execve(path, exec_create_command(expanded), NULL);
	free(path);
	exec_print_error_exit(CMD_NOT_FOUND, expanded->token);
}
