/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main_flow.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:54 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/09 13:31:09 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

// execveに送る場合
// {"ls", "-l", "NULL"}の状態にする。
char	**ms_create_execute_command(t_stree *tree)
{
	char	**command;
	size_t	i;
	t_stree	*tmp;

	i = ms_get_cmd_size(tree);
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
int	ms_expand_and_redirect(t_clause *clause)
{
	t_redir	*rd;
	t_redir *expand_rd;

	rd = clause->redir;
	while (rd) // 逐次的にエキスパンドとリダイレクトを行う  echo hello > $VAR > b > c | cat
	{
		expand_rd = do_expander(rd); // リダイレクションを展開する (echo a > $VAR　-> echo a > var)
		if (ms_redirect(expand_rd) == 1)// リダイレクションを処理する
		{
			ms_check_fd_print_error(expand_rd);
			free(expand_rd);
			break ;
		}
		free(expand_rd);
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

// ">,>>" があったらとりあえず先に上書きとファイル作成を行う。
// 不正なfdでもここではエラー処理はしない
void	ms_just_open_file(t_clause *clause)
{
	t_clause	*tmp_cl;
	t_redir		*tmp_re;

	tmp_cl = clause;
	while (tmp_cl)
	{
		tmp_re = tmp_cl->redir;
		while (tmp_re)
		{
			if (tmp_re->redir_op == TI_GT)
			{
				if (open(tmp_re->operand_right, O_WRONLY | O_CREAT | O_TRUNC, 0666) == -1); // Permissionなどでopenに失敗したらそれ以降のそれ以降の処理はしない
					break ;
			}
			if (tmp_re->redir_op == TI_GTGT)
			{
				if (open(tmp_re->operand_right, O_WRONLY | O_CREAT | O_TRUNC, 0666) == -1); // Permissionなどでopenに失敗したらそれ以降のそれ以降の処理はしない
					break ;
			}
			if (tmp_re->redir_op == TI_LT)
			{
				if (open(tmp_re->operand_right, O_RDONLY) == -1)
					break ;
			}
			if (ms_check_fd(tmp_re->operand_left) < 0) // 不正なfdだった場合それ以降のリダイレクトは処理しない
				break ;
			tmp_re = tmp_re->next;
		}
		tmp_cl = tmp_cl->next;
	}
}

// パイプラインを再帰的に処理する
int	ms_executer(t_pipeline *pl, t_shellvar *var, t_ex_state *state)
{
	t_dpipe *dpipe;
	int	status;

	if (pl == NULL)
		return (0);
	ms_just_open_file(pl->clause);
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
