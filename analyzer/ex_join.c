/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_join.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/04 18:39:58 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/07 11:15:16 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

static int	ex_quote_involved(t_ex_token *temp)
{
	while (temp && temp->token)
	{
		if (temp->token_id == XI_SQUOTED)
			return (1);
		if (temp->token_id == XI_DQUOTED)
			return (1);
		temp = temp->right;
	}
	return (0);
}

static t_stree	*ex_join_into(t_ex_state *state, t_ex_unit_cursor *csr,
		t_ex_token *temp)
{
	char	*joined;
	t_stree	*st;

	joined = ex_strcat_exlist(temp, 0);
	if (!joined)
	{
		ex_mark_failed(state, 1, "[JO] join splitted token");
		return (NULL);
	}
	st = ex_make_stree(joined, temp->pa_token_id);
	if (!st)
	{
		free(joined);
		ex_mark_failed(state, 1, "[JO] make a joined stree");
		return (NULL);
	}
	st->quote_involved = ex_quote_involved(temp);
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
