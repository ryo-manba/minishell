/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_fx.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 05:54:52 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/05 05:54:52 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

size_t	ex_exlist_size(t_ex_unit_cursor *cursor, t_ex_token *head)
{
	t_ex_token	*temp;
	size_t		n;

	n = 0;
	temp = head;
	while (temp != cursor->s.tail)
	{
		n += 1;
		printf("n = %zu, %p\n", n, temp);
		printf("%s\n", temp->token);
		temp = temp->right;
	}
	printf("n = %zu\n", n);
	return (n);
}

int	ex_fx_extdup(t_ex_state *state, t_ex_unit_cursor *cursor,
	t_ex_token *head)
{
	t_ex_token	*temp;

	temp = head;
	while (!state->failed)
	{
		if (temp == cursor->s.tail)
			break ;
		cursor->vs = 0;
		cursor->i = ft_strlen(temp->token);
		ex_clone_and_push_back_token(state, cursor, temp);
		temp = temp->right;
	}
	return (!!state->failed);
}

t_ex_token	*ex_fx_terminate(t_ex_state *state, t_ex_unit_cursor *cursor,
	t_ex_token *temp)
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
		state->failed = 1;
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
	t_ex_token			*temp;

	ex_init_cursor_mid(&csr, token);
	temp = NULL;
	while (!state->failed)
	{
		if (csr.s.tail && csr.s.tail->token_id != XI_DIVIDER)
		{
			if (!temp)
				temp = csr.s.tail;
		}
		else
			ex_fx_terminate(state, &csr, temp);
		if (!csr.s.tail)
			break ;
		csr.s.tail = csr.s.tail->right;
	}
	ex_destroy_token(csr.s.head);
	if (state->failed)
	{
		ex_destroy_token(csr.p.head);
		csr.p.head = NULL;
	}
	return (csr.p.head);
}
