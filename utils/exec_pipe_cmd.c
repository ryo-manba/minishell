/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:38 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/19 11:46:03 by yokawada         ###   ########.fr       */
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
	t_pipeline *pl, t_master *master, t_ex_state *es, t_dpipe *dpipe)
{
	t_stree	*expanded;

	ms_do_piping(pl->clause, dpipe->new, dpipe->before);
	if (pl->clause->stree && pl->clause->stree->subshell)
	{
		exec_subshell(pl->clause, master, es);
		exit(g_ex_states);
	}
	expanded = ms_expand_stree(es, pl->clause->stree);
	if (!expanded && es->failed == 0 && !pl->clause->redir)
		exit(0);
	if (!expanded && es->failed)
		exit(1);
	g_ex_states = exec_expand_redirect(es->master, pl->clause);
	if (g_ex_states != MS_BLT_SUCC || !expanded)
		exit(g_ex_states);
	if (ms_is_builtin(expanded))
		exit(ms_exec_builtin(expanded, es->master));
	else
		exec_run_cmd_exit(es->master, expanded, master->var);
}

// パイプが繋がっていた場合のコマンド実行の処理
// 最後のコマンドのpidからステータスを取る
int	exec_pipe_command(t_pipeline *pl, t_master *master, t_ex_state *state)
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
			exec_pipe_child(pl, master, state, &dpipe);
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
void	exec_run_cmd_exit(t_master *master, t_stree *expanded, t_shellvar *var)
{
	char	*path;
	char	**env;
	int		is_relative;

	is_relative = 1;
	env = exec_restore_env(var);
	if (env == NULL)
		ms_perror_exit("malloc");
	if (ft_strchr_i(expanded->token, '/') != -1)
	{
		path = ft_strdup(expanded->token);
		if (path == NULL)
			ms_perror_exit("malloc");
		is_relative = 0;
	}
	else
		path = exec_get_path(expanded->token, var);
	exec_check_path_exit(master, expanded, path);
	execve(path, exec_create_command(expanded), env);
	exec_all_free(env);
	exec_check_path(path, is_relative);
	exec_check_path_exit(master, expanded, path);
	free(path);
	exec_print_error_exit(master, CMD_NOT_FOUND, expanded->token);
}
