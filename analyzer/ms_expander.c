/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:19:44 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/07 12:56:23 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

static void	make_state_for_redir(t_ex_state *state, t_redir *redir)
{
	state->no_param =
		(redir->redir_op == TI_LTLT || redir->redir_op == TI_LTLTHYPHEN);
	state->no_file =
		(redir->redir_op == TI_LTLT || redir->redir_op == TI_LTLTHYPHEN);
}

static void	reverse_state_for_redir(t_ex_state *state)
{
	state->no_param = 0;
	state->no_file = 0;
}

static void	concat_stree_cursor(t_ex_cursor *cursor, t_stree *res)
{
	if (!res)
		return ;
	if (!cursor->res.head)
		cursor->res.head = res;
	if (!cursor->res.tail)
		cursor->res.tail = res;
	else
		cursor->res.tail->right = res;
	while (cursor->res.tail->right)
		cursor->res.tail = cursor->res.tail->right;
}

// expand **ONLY** a given redir
t_redir	*ms_expand_a_redir(t_ex_state *state, t_redir *redir)
{
	t_redir	*cloned;
	t_stree	*original;

	original = redir->operand_right;
	if (!original)
		return ((t_redir *)ex_error(state, NULL, "*UNEXPECTED blank redir*"));
	cloned = (t_redir *)ft_calloc(1, sizeof(t_redir));
	if (!cloned)
		return ((t_redir *)ex_error(state, NULL, "bad alloc"));
	cloned->redir_op = redir->redir_op;
	make_state_for_redir(state, redir);
	cloned->operand_right = ms_expand_stree(state, redir->operand_right);
	reverse_state_for_redir(state);
	if (state->failed || !cloned->operand_right || original->right)
	{
		pa_destroy_redir(cloned);
		return ((t_redir *)ex_error(state, original, "ambiguous redirect"));
	}
	cloned->operand_left = ms_expand_stree(state, redir->operand_left);
	if (state->failed || (cloned->operand_left && cloned->operand_left->left))
	{
		pa_destroy_redir(cloned);
		return ((t_redir *)ex_error(state, original, "ambiguous redirect"));
	}
	return (cloned);
}

t_stree	*ms_expand_stree(t_ex_state *state, t_stree *src)
{
	t_ex_cursor	cursor;
	t_ex_token	*res;
	t_stree		*st;

	ft_bzero(&cursor, sizeof(t_ex_cursor));
	cursor.src.head = src;
	cursor.src.tail = src;
	while (cursor.src.tail)
	{
		res = ex_shell_param(state, cursor.src.tail);
		if (!state->no_split || cursor.src.tail->token_id != TI_ASSIGNMENT_WORD)
			res = ex_split(state, res);
		if (cursor.src.tail->token_id != TI_ASSIGNMENT_WORD)
			res = ex_fx(state, res);
		st = ex_join(state, res);
		concat_stree_cursor(&cursor, st);
		cursor.src.tail = cursor.src.tail->right;
	}
	return (cursor.res.head);
}
