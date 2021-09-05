/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pa_syntax_term.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:21:44 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/06 00:21:44 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

char	*pa_syntax_final(t_parse_state *state)
{
	if (!state->cursor.clause->stree)
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
		return (pa_syntax_error(state, cursor->word, "NO CLAUSE"));
	if (!clause->stree && !clause->redir)
	{
		if (!by_newline)
			return (pa_syntax_error(state, cursor->word, "BLANK_CLAUSE"));
		if (cursor->expecting_continuation)
			return (pa_syntax_error(state,
					cursor->word, "EXPECTED_CONTINUATION"));
	}
	return (MS_AZ_SUCC);
}

int	pa_syntax_term_pipeline(t_parse_state *state, int by_newline)
{
	if (pa_syntax_term_clause(state, by_newline))
		return (MS_AZ_FAIL);
	return (MS_AZ_SUCC);
}
