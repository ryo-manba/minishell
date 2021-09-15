/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pa_unit_operator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:21:48 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/13 02:44:33 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

int	pa_subshell_enter(t_parse_state *state, t_wdlist *word)
{
	t_parse_state	substate;
	t_stree			*st;

	if (!state->cursor.word->next)
		return (pa_syntax_error(state, word, "'(' needs right elem)"));
	if (state->cursor.stree)
		return (pa_syntax_error(state, word, "'(' is not leading"));
	if (state->cursor.redir)
		return (pa_syntax_error(state, word, "REDIR_BEFORE_SUBSHELL"));
	if (ms_parse(state->master, &substate, word->next, 1))
	{
		state->error_printed = state->error_printed || substate.error_printed;
		return (pa_generic_error(state, word, "bad alloc subshell"));
	}
	if (substate.err_message && substate.err_word)
		return (pa_syntax_error(state,
				substate.err_word, substate.err_message));
	state->cursor.word = substate.cursor.word;
	st = pa_make_stree(word, 1);
	if (!st)
		return (pa_generic_error(state, word, "bad alloc stree"));
	st->subshell = substate.pipeline;
	if (!pa_add_stree(state, st))
		return (pa_generic_error(state, word, "bad alloc stree"));
	return (MS_AZ_SUCC);
}

int	pa_subshell_leave(t_parse_state *state, t_wdlist *word)
{
	char		*final_error;

	if (!state->for_subshell)
		return (pa_syntax_error(state, word, "UNEXPECTED_SUBSHELL_CLOSER"));
	if (!state->cursor.redir && !state->cursor.stree)
		return (pa_syntax_error(state, word, "UNEXPECTED_SUBSHELL_CLOSER"));
	final_error = pa_syntax_final(state);
	if (final_error)
		return (pa_syntax_error(state, word, final_error));
	state->finished = 1;
	return (MS_AZ_SUCC);
}

int	pa_terminate_pipeline(t_parse_state *state, t_wdlist *word)
{
	t_token_id	term;

	if (pa_syntax_term_pipeline(state, 0))
		return (pa_syntax_error(state, word, "syntax error in pipeline"));
	term = pa_operator_token_id(word);
	state->cursor.pipeline->joint = term;
	if (!pa_add_new_pipeline(state))
		return (pa_generic_error(state, word, "bad alloc pipeline"));
	state->cursor.expecting_continuation
		= (term == TI_ANDAND || term == TI_PIPEPIPE);
	return (MS_AZ_SUCC);
}

int	pa_terminate_clause(t_parse_state *state, t_wdlist *word)
{
	t_token_id	term;

	if (pa_syntax_term_clause(state, 0))
		return (pa_syntax_error(state, word, "syntax error in clause"));
	if (!pa_add_new_clause(state))
		return (pa_generic_error(state, word, "bad alloc clause"));
	term = pa_operator_token_id(word);
	state->cursor.expecting_continuation = (term == TI_PIPE);
	return (MS_AZ_SUCC);
}
