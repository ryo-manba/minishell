/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_fx.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 05:54:52 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/30 21:32:10 by yokawada         ###   ########.fr       */
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
	ex_stringify_extoken_ifneeded(csr->p.head, "[FX]");
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

size_t	ex_fx_exlist_len(t_ex_token *ext)
{
	size_t		n;
	t_ex_token	*temp;

	n = 0;
	temp = ext;
	while (temp && temp->token)
	{
		n += ft_strlen(temp->token);
		temp = temp->right;
	}
	return (n);
}

int	ex_fx_list_is_for_fx(t_ex_token *ext)
{
	t_ex_token	*temp;

	temp = ext;
	while (temp && temp->token)
	{
		if (temp->token_id != XI_DQUOTED && temp->token_id != XI_SQUOTED)
		{
			if (!!ft_strchr((char *)temp->token, '*'))
				return (1);
		}
		temp = temp->right;
	}
	return (0);
}

t_ex_token	*ex_fx_terminate(t_ex_state *state,
	t_ex_unit_cursor *cursor, t_ex_token *temp)
{
	char	*joined;
	size_t	matched_count;
	size_t	mm;

	if (!temp)
	{
		if (cursor->s.tail && cursor->s.tail->token_id == XI_DIVIDER)
			ex_push_back_divider_if_needed(state, cursor, temp);
		return (NULL);
	}
	joined = ex_strcat_exlist(temp, 0);
	if (!joined)
		ex_mark_failed(state, 1, "[FX] join ex-fx pattern");
	mm = ex_fx_exlist_len(temp);
	matched_count = 0;
	if (!state->failed && ex_fx_list_is_for_fx(temp))
	{
		cursor->running = temp->token_id;
		cursor->pa_token_id = temp->pa_token_id;
		matched_count = ex_fx_expand_lst(state, cursor, temp, mm);
		// matched_count = ex_fx_expand(state, cursor, joined, ft_strlen(joined));
	}
	free(joined);
	if (!state->failed && matched_count == 0)
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
