/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_just_open.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 20:14:25 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/10 23:14:10 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

// 失敗するか不正なfdが出現するまでファイルをopenする
void	exec_all_open(t_redir *expand_rd)
{
	const int	gt = O_WRONLY | O_CREAT | O_TRUNC;
	const int	gtgt = O_WRONLY | O_CREAT | O_APPEND;
	const int	lt = O_RDONLY;
	t_redir		*tmp_rd;

	tmp_rd = expand_rd;
	while (tmp_rd)
	{
		if (tmp_rd->redir_op == TI_GT && \
			open(tmp_rd->operand_right->token, gt, 0666) == -1)
			break ;
		if (tmp_rd->redir_op == TI_GTGT && \
			open(tmp_rd->operand_right->token, gtgt, 0666) == -1)
			break ;
		if (tmp_rd->redir_op == TI_LT && \
			open(tmp_rd->operand_right->token, lt) == -1)
			break ;
		if (tmp_rd->operand_left && \
			ms_check_fd(tmp_rd->operand_left->token) > 0)
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
		if (tmp_cl->redir && \
			(tmp_cl->redir->redir_op == TI_GT || \
			tmp_cl->redir->redir_op == TI_GTGT))
		{
			expanded_rd = ms_expand_a_redir(&es, tmp_cl->redir);
			exec_all_open(expanded_rd);
			pa_destroy_redir(expanded_rd);
		}
		tmp_cl = tmp_cl->next;
	}
}
