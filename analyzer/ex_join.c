/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_join.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/04 18:39:58 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/05 00:23:23 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

static t_stree	*ex_join_into(t_ex_state *state, t_ex_unit_cursor *csr,
		t_ex_token *temp)
{
	char	*joined;
	t_stree	*st;

	joined = ex_strcat_exlist(temp, 0);
	if (!joined)
	{
		state->failed = 1;
		return (NULL);
	}
	st = ex_make_stree(joined, temp->pa_token_id);
	if (!st)
	{
		free(joined);
		state->failed = 1;
		return (NULL);
	}
	if (csr->t.tail)
		csr->t.tail->right = st;
	csr->t.tail = st;
	if (!csr->t.head)
		csr->t.head = csr->t.tail;
	return (st);
}

static t_stree	*ex_jo_destroy_cursor(t_ex_state *state, t_ex_unit_cursor *csr)
{
	ex_destroy_token(csr->s.head);
	if (state->failed)
	{
		pa_destroy_stree(csr->t.head);
		return (NULL);
	}
	return (csr->t.head);
}

t_stree	*ex_join(t_ex_state *state, t_ex_token *ext)
{
	t_ex_unit_cursor	csr;
	t_ex_token			*temp;

	ex_init_cursor_mid(&csr, ext);
	temp = NULL;
	while (!state->failed)
	{
		if (csr.s.tail && csr.s.tail->token_id != XI_DIVIDER)
		{
			if (!temp)
				temp = csr.s.tail;
		}
		else if (temp)
		{
			if (!ex_join_into(state, &csr, temp))
				break ;
			temp = NULL;
		}
		if (!csr.s.tail)
			break ;
		csr.s.tail = csr.s.tail->right;
	}
	return (ex_jo_destroy_cursor(state, &csr));
}
