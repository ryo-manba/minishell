/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_ll_dquote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/03 21:31:49 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/03 21:31:51 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

// concatenate all strings in given ex_token list.
static char	*ex_strcat_exlist(t_ex_token *head, size_t s)
{
	size_t		n;
	char		*joined;

	if (!head)
	{
		joined = (char *)malloc((s + 1) * sizeof(char));
		if (!joined)
			return (NULL);
		joined[s] = '\0';
		return (joined);
	}
	n = ft_strlen(head->token);
	joined = ex_strcat_exlist(head->right, s + n);
	if (!joined)
		return (NULL);
	ft_memcpy(joined + s, head->token, n);
	return (joined);
}

int	ex_ll_trap_dquote(t_ex_state *state, t_ex_unit_cursor *csr)
{
	t_ex_unit_cursor	cursor;
	char				*joined;

	csr->i += 1;
	ex_ll_init_cursor(&cursor, csr->pa_token_id, csr->str + csr->i, '"');
	ex_ll_unit(state, &cursor);
	if (state->failed)
		return (MS_AZ_FAIL);
	ex_stringify_extoken(cursor.p.head);
	joined = ex_strcat_exlist(cursor.p.head, 0);
	ex_destroy_token(cursor.p.head);
	if (!joined)
		return (MS_AZ_FAIL);
	csr->running = XI_DQUOTED;
	if (!ex_push_back_token(state, csr, joined))
		return (MS_AZ_FAIL);
	csr->running = XI_NEUTRAL;
	csr->i += cursor.i + 1;
	return (MS_AZ_SUCC);
}
