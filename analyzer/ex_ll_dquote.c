/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_ll_dquote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/03 21:31:49 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/19 11:44:02 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

// concatenate all strings in given ex_token list.
char	*ex_strcat_exlist(t_ex_token *head, size_t s)
{
	size_t		n;
	char		*joined;

	if (!head || !head->token)
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
	t_ex_unit_cursor	dcursor;
	char				*joined;
	int					nul_terminated;

	ex_ll_init_cursor(&dcursor, csr->pa_token_id, csr->str + csr->i, '"');
	nul_terminated = !dcursor.str[dcursor.n];
	ex_ll_unit(state, &dcursor);
	if (state->failed)
		return (MS_AZ_FAIL);
	joined = ex_strcat_exlist(dcursor.p.head, 0);
	ex_destroy_token(dcursor.p.head);
	if (!joined)
		return (MS_AZ_FAIL);
	csr->running = XI_DQUOTED;
	if (!ex_push_back_token(state, csr, joined))
	{
		free(joined);
		return (MS_AZ_FAIL);
	}
	csr->running = XI_NEUTRAL;
	csr->i += dcursor.n + !nul_terminated;
	return (MS_AZ_SUCC);
}

// for heredoc, ONLY expand variables in a given line.
// ignore all quotations.
char	*ex_ll_heredoc_line(t_ex_state *es, char *line)
{
	t_ex_unit_cursor	cursor;
	t_ex_state			hstate;
	char				*joined;

	ms_ex_init_state(&hstate, es->master);
	ex_ll_init_cursor(&cursor, TI_WORD, line, '\0');
	hstate.ignore_quote = 1;
	ex_ll_unit(&hstate, &cursor);
	if (hstate.failed)
		return (NULL);
	joined = ex_strcat_exlist(cursor.p.head, 0);
	ex_destroy_token(cursor.p.head);
	if (!joined)
		return (NULL);
	return (joined);
}
