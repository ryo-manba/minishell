/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:38 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/13 23:23:06 by rmatsuka         ###   ########.fr       */
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
	g_ex_states = exec_expand_redirect(pl->clause, var);
	if (g_ex_states != MS_BLT_SUCC)
		exit(g_ex_states);
	if (pl->clause->stree->subshell != NULL)
		ms_executer(pl->clause->stree->subshell, var, es);
	expanded = ms_expand_stree(es, pl->clause->stree);
	if (!expanded && es->failed == 0)
		exit(0);
	if (!expanded && es->failed)
		exit(1);
	if (ms_is_builtin(expanded))
		exit(ms_exec_builtin(var, expanded));
	else
		exec_run_cmd_exit(expanded, var);
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
	exec_set_signal_wait(pid);
	return (g_ex_states);
}

// 親でpipeを閉じる
void	exec_pipe_parent(t_dpipe *dpipe)
{
	ms_close_and_update_pipe(dpipe->new, dpipe->before);
}

// '/' が含まれてる場合はそのまま実行する
// ない場合はPATHから探す
void	exec_run_cmd_exit(t_stree *expanded, t_shellvar *var)
{
	char	*path;
	char	**env;

	env = exec_restore_env(var);
	if (env == NULL)
		ms_perror_exit("malloc");
	if (ft_strchr_i(expanded->token, '/') != -1)
	{
		path = ft_strdup(expanded->token);
		if (path == NULL)
			ms_perror_exit("malloc");
	}
	else
		path = exec_get_path(expanded->token, var);
	if (exec_check_path_state(expanded, path) == MS_EXEC_FAIL)
		exec_print_error_exit(NO_SUCH_FILE, NULL);
	execve(path, exec_create_command(expanded), env);
	free(path);
	exec_all_free(env);
	exec_print_error_exit(CMD_NOT_FOUND, expanded->token);
}
