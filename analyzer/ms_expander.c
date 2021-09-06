/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:19:44 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/06 22:15:59 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

static void	init_ex_cursor(t_ex_cursor *cursor, t_stree *src)
{
	ft_bzero(cursor, sizeof(t_ex_cursor));
	cursor->src.head = src;
	cursor->src.tail = src;
}

void	concat_stree_cursor(t_ex_cursor *cursor, t_stree *res)
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
	t_stree	*expanded;

	cloned = (t_redir *)malloc(sizeof(t_redir));
	if (!cloned)
		return (NULL);
	ft_memcpy(cloned, redir, sizeof(t_redir));
	cloned->next = NULL;
	original = cloned->operand_right;
	if (!original)
	{
		free(cloned);
		return ((t_redir *)ex_error(state, NULL, "*UNEXPECTED blank redir*"));
	}
	expanded = ms_expand_stree(state, original);
	if (!expanded || expanded->right)
	{
		free(cloned);
		return ((t_redir *)ex_error(state, original, "ambiguous redirect"));
	}
	cloned->operand_right = expanded;
	return (cloned);
}

t_stree	*ms_expand_stree(t_ex_state *state, t_stree *src)
{
	t_ex_cursor	cursor;
	t_ex_token	*res;
	t_stree		*st;

	init_ex_cursor(&cursor, src);
	while (cursor.src.tail)
	{
		res = ex_shell_param(state, cursor.src.tail);
		ex_stringify_extoken(res);
		if (!state->no_split || cursor.src.tail->token_id != TI_ASSIGNMENT_WORD)
			res = ex_split(state, res);
		ex_stringify_extoken(res);
		if (cursor.src.tail->token_id != TI_ASSIGNMENT_WORD)
			res = ex_fx(state, res);
		ex_stringify_extoken(res);
		st = ex_join(state, res);
		concat_stree_cursor(&cursor, st);
		cursor.src.tail = cursor.src.tail->right;
	}
	return (cursor.res.head);
}
