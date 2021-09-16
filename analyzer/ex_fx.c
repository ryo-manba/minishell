/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_fx.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 05:54:52 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/17 00:14:09 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

static t_ex_token	*ex_fx_exit(t_ex_state *state, t_ex_unit_cursor *csr)
{
	ex_destroy_token(csr->s.head);
	if (state->failed)
	{
		ex_destroy_token(csr->p.head);
		csr->p.head = NULL;
	}
	else if (PA_DEBUG)
		ex_stringify_extoken(csr->p.head);
	return (csr->p.head);
}

static int	ex_fx_extdup(t_ex_state *state, t_ex_unit_cursor *cursor,
	t_ex_token *head)
{
	t_ex_token	*temp;

	temp = head;
	while (!state->failed)
	{
		if (temp && temp->token_id == XI_DIVIDER)
			ex_push_back_divider_if_needed(state, cursor, temp);
		else if (temp)
		{
			cursor->vs = 0;
			cursor->i = ft_strlen(temp->token);
			ex_clone_and_push_back_token(state, cursor, temp);
		}
		if (temp == cursor->s.tail)
			break ;
		temp = temp->right;
	}
	return (!!state->failed);
}

static t_ex_token	*ex_fx_terminate(t_ex_state *state,
	t_ex_unit_cursor *cursor, t_ex_token *temp)
{
	char	*joined;
	size_t	m;

	if (!temp)
	{
		if (cursor->s.tail && cursor->s.tail->token_id == XI_DIVIDER)
			ex_push_back_divider_if_needed(state, cursor, temp);
		return (NULL);
	}
	joined = ex_strcat_exlist(temp, 0);
	if (!joined)
		ex_mark_failed(state, 1, "[FX] join ex-fx pattern");
	m = 0;
	if (!state->failed && ft_strchr(joined, '*'))
	{
		cursor->running = temp->token_id;
		cursor->pa_token_id = temp->pa_token_id;
		m = ex_fx_expand(state, cursor, joined, ft_strlen(joined));
	}
	free(joined);
	if (!state->failed && m == 0)
		ex_fx_extdup(state, cursor, temp);
	return (cursor->p.tail);
}

t_ex_token	*ex_fx(t_ex_state *state, t_ex_token *token)
{
	t_ex_unit_cursor	csr;
	t_ex_token			*sublist_head;

	ex_init_cursor_mid(&csr, token);
	sublist_head = NULL;
	while (!state->failed)
	{
		if (csr.s.tail && csr.s.tail->token_id != XI_DIVIDER)
		{
			if (!sublist_head)
				sublist_head = csr.s.tail;
		}
		else
		{
			ex_fx_terminate(state, &csr, sublist_head);
			sublist_head = NULL;
		}
		if (!csr.s.tail)
			break ;
		csr.s.tail = csr.s.tail->right;
	}
	return (ex_fx_exit(state, &csr));
}
