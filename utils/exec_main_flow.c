/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main_flow.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:54 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/11 14:24:32 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

// execveに送る場合
// {"ls", "-l", "NULL"}の状態にする。
char	**exec_create_command(t_stree *tree)
{
	char	**command;
	size_t	i;
	t_stree	*tmp;

	i = exec_get_command_size(tree);
	command = (char **)malloc(sizeof(char *) * i + 1);
	if (command == NULL)
		ms_print_perror_exit("malloc");
	tmp = tree;
	i = 0;
	while (tmp)
	{
		command[i] = ft_strdup(tmp->token);
						printf("[%zu], [%s]\n", i, command[i]);　// DEBUG
		if (command[i] == NULL)
		{
			exec_all_free(command);
			ms_print_perror("malloc");
		}
		tmp = tmp->right;
		i++;
	}
	command[i] = NULL;
	return (command);
}

// 環境変数を展開しながらリダイレクションを処理する
int	exec_expand_redirect(t_clause *clause, t_shellvar *var)
{
	t_redir		*rd;
	t_redir		*expanded_rd;
	t_ex_state	es;
	int			err;

	ms_ex_init_state(&es, var, 0);
	rd = clause->redir;
	err = 0;
	while (rd)
	{
		expanded_rd = ms_expand_a_redir(&es, rd);
		if (!expanded_rd)
			return (MS_EXEC_FAIL);
		if (ms_redirect(expanded_rd, var) == MS_EXEC_FAIL)
		{
			err = ms_check_fd_print_error(expanded_rd);
			pa_destroy_redir(expanded_rd);
			return (err);
		}
		pa_destroy_redir(expanded_rd);
		rd = rd->next;
	}
	return (MS_EXEC_SUCC);
}

void	exec_update_exitstatus(t_ex_state *state, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	state->last_exit_status = WEXITSTATUS(status);
}

// '&&','||' で条件を満たしている場合に再帰的に実行する
// fork->pipe->redirect->execの順
int	ms_executer(t_pipeline *pl, t_shellvar *var, t_ex_state *state)
{
	if (pl == NULL)
		return (0);
	state->var = var;
	if (exec_just_open(pl->clause, var))
		return (1);
	if (pl->clause->next != NULL)
		exec_pipe_command(pl, var, state);
	else
		state->last_exit_status = exec_simple_command(pl->clause, var, state);
	if ((pl->joint == TI_ANDAND && state->last_exit_status == 0)
		|| (pl->joint == TI_PIPEPIPE && state->last_exit_status == 1))
	{
		ms_executer(pl->next, var, state);
	}
	return (0);
}
