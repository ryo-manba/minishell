/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main_flow.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:54 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/10 03:35:23 by yokawada         ###   ########.fr       */
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
	{
		ms_print_perror("malloc");
		return (NULL);
	}
	tmp = tree;
	i = 0;
	while (tmp)
	{
		command[i] = ft_strdup(tmp->token);
		if (command[i] == NULL)
		{
			ms_all_free(command);
			ms_print_perror("malloc");
			return (NULL);
		}
		tmp = tmp->right;
		i++;
	}
	command[i] = NULL;
	return (command);
}

// 環境変数を展開しながらリダイレクションを処理する
// エクスパンダーの部分修正する
int	exec_expand_redirect(t_clause *clause, t_shellvar *var)
{
	t_redir		*rd;
	t_redir		*expanded_rd;
	t_ex_state	es;

	ms_ex_init_state(&es, var, 0);
	rd = clause->redir;
	while (rd) // 逐次的にエキスパンドとリダイレクトを行う  echo hello > $VAR > b > c | cat
	{
		expanded_rd = ms_expand_a_redir(NULL, rd); // リダイレクションを展開する (echo a > $VAR　-> echo a > var)
		if (ms_redirect(expanded_rd) == 1)// リダイレクションを処理する
		{
			ms_check_fd_print_error(expanded_rd);
			pa_destroy_redir(expanded_rd);
			break ;
		}
		pa_destroy_redir(expanded_rd);
		rd = rd->next;
	}
	if (rd != NULL) // リダイレクトが最後まで処理されていない場合
		return (1);
	return (0);
}

void	ms_update_exitstatus(t_ex_state *state, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	state->last_exit_status = WEXITSTATUS(status);
}

// openに失敗するまでファイル作成する
void	exec_all_open(t_redir *expand_rd)
{
	t_redir	*tmp_rd;

	tmp_rd = expand_rd;
	while (tmp_rd)
	{
		if (tmp_rd->redir_op == TI_GT)
		{
			if (open(
				tmp_rd->operand_right, O_WRONLY | O_CREAT | O_TRUNC, 0666) == -1); // Permissionなどでopenに失敗したらそれ以降のそれ以降の処理はしない
				break ;
		}
		if (tmp_rd->redir_op == TI_GTGT)
		{
			if (open(
				tmp_rd->operand_right, O_WRONLY | O_CREAT | O_APPEND, 0666) == -1); // Permissionなどでopenに失敗したらそれ以降のそれ以降の処理はしない
				break ;
		}
		if (tmp_rd->redir_op == TI_LT)
		{
			if (open(tmp_rd->operand_right, O_RDONLY) == -1)
				break ;
		}
		if (ms_check_fd(tmp_rd->operand_left) < 0) // 不正なfdだった場合それ以降のリダイレクトは処理しない
			break ;
		tmp_rd = tmp_rd->next;
	}
}

// ">,>>" があったらとりあえず先に上書きとファイル作成を行う。
// 不正なfdや権限で失敗してもここではエラーを出さない
void	exec_just_open(t_clause *clause, t_shellvar *var)
{
	t_ex_state	es;
	t_clause	*tmp_cl;
	t_redir		*expanded_rd;

	ms_ex_init_state(&es, var, 0);
	tmp_cl = clause;
	while (tmp_cl)
	{
		if (tmp_cl->redir)
		{
			expanded_rd = ms_expand_a_redir(&es, tmp_cl->redir);
			exec_all_open(expanded_rd);
			pa_destroy_redir(expanded_rd);
		}
		tmp_cl = tmp_cl->next;
	}
}

// パイプラインを再帰的に処理する
int	ms_executer(t_pipeline *pl, t_shellvar *var, t_ex_state *state)
{
	t_dpipe	*dpipe;

	if (pl == NULL)
		return (0);
	if (!var)
		var = state->var;
	if (!var)
		var = ms_create_env();
	if (!var)
		return (1);
	state->var = var;
	exec_just_open(pl->clause, var);
	if (pl->clause->next != NULL) // パイプがある場合、終わるまでループ回す
		exec_pipe_command(pl, var, state);
	else
		state->last_exit_status = exec_simple_command(pl->clause, var);
	if (pl->joint == TI_ANDAND && state->last_exit_status == 0 // && 前のコマンドが成功した場合
		|| pl->joint == TI_PIPEPIPE && state->last_exit_status == 1) // || 前のコマンドが失敗した場合
	{
		ms_executer(pl->next, var, state); // '&&','||' で条件を満たしている場合に再帰的に実行する
	}
	return (0);
}
