/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pa_syntax_term.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:21:44 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/18 01:22:40 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

char	*pa_syntax_final(t_parse_state *state)
{
	if (!state->cursor.clause || !state->cursor.clause->stree)
		if (state->cursor.expecting_continuation)
			return ("EXPECTED_CONTINUATION");
	return (NULL);
}

int	pa_syntax_term_clause(t_parse_state *state, int by_newline)
{
	t_parse_cursor	*cursor;
	t_clause		*clause;

	cursor = &(state->cursor);
	clause = cursor->clause;
	if (!clause)
		return (MS_AZ_FAIL);
	if (!clause->stree && !clause->redir)
	{
		if (!by_newline)
			return (MS_AZ_FAIL);
		if (cursor->expecting_continuation)
			return (MS_AZ_FAIL);
	}
	return (MS_AZ_SUCC);
}

int	pa_syntax_term_pipeline(t_parse_state *state, int by_newline)
{
	if (pa_syntax_term_clause(state, by_newline))
		return (MS_AZ_FAIL);
	return (MS_AZ_SUCC);
}
