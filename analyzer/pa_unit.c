/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pa_unit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:22:13 by yokawada          #+#    #+#             */
/*   Updated: 2021/10/01 18:34:24 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

static int	pa_unit_newline(t_parse_state *state, t_wdlist *word)
{
	char	*final_error;

	if (state->for_subshell)
		return (pa_syntax_error(state, word, "NEWLINE_IN_SUBSHELL"));
	final_error = pa_syntax_final(state);
	if (final_error)
		return (pa_syntax_error(state, word, final_error));
	state->finished = 1;
	return (1);
}

static int	pa_unit_io_number(t_parse_state *state, t_wdlist *word)
{
	t_stree		*st;
	t_wdlist	*next_word;

	next_word = pa_shift_lx_token(state);
	if (!next_word)
		return (pa_syntax_error(state, word, "SOLE_IO_NUMBER"));
	st = pa_make_stree(word, 0);
	if (!st)
		return (pa_generic_error(state, word, "bad alloc"));
	if (pa_redirection(state, next_word, st))
		return (pa_generic_error(state, next_word, "bad alloc"));
	return (MS_AZ_SUCC);
}

static int	pa_unit_token(t_parse_state *state, t_wdlist *word)
{
	t_stree	*st;

	st = pa_make_stree(word, 0);
	if (!st)
		return (pa_generic_error(state, word, "bad alloc"));
	if (state->cursor.stree && state->cursor.stree->token_id == TI_SUBSHELL)
		return (pa_syntax_error(state, word, "NEXT_TO_SUBSHELL"));
	if (lx_str_is_for_assignment_word(word->word, word->len))
		st->token_id = TI_ASSIGNMENT_WORD;
	if (!pa_add_stree(state, st))
		return (MS_AZ_FAIL);
	return (MS_AZ_SUCC);
}

static int	pa_unit_operator(t_parse_state *state, t_wdlist *word)
{
	t_token_id	ti;

	ti = pa_operator_token_id(state, word);
	if (ti == TI_PAREN_L)
		return (pa_subshell_enter(state, word));
	if (ti == TI_PAREN_R)
		return (pa_subshell_leave(state, word));
	if (ti == TI_ANDAND || ti == TI_PIPEPIPE || ti == TI_AND
		|| ti == TI_SEMICOLON)
		return (pa_terminate_pipeline(state, word));
	if (ti == TI_PIPE)
		return (pa_terminate_clause(state, word));
	if (ti == TI_LT || ti == TI_GT || ti == TI_LTLT || ti == TI_GTGT
		|| ti == TI_LTGT || ti == TI_LTAND || ti == TI_GTAND
		|| ti == TI_LTLTHYPHEN)
		return (pa_redirection(state, word, NULL));
	return (pa_syntax_error(state, word, "NOT_CAPTURED"));
}

int	pa_unit(t_parse_state *state)
{
	t_wdlist	*word;

	word = pa_shift_lx_token(state);
	if (!word)
		return (1);
	if (word->lex_type == LT_NEWLINE)
		return (pa_unit_newline(state, word));
	else if (word->lex_type == LT_IO_NUMBER)
		return (pa_unit_io_number(state, word));
	else if (word->lex_type == LT_TOKEN)
		return (pa_unit_token(state, word));
	else if (word->lex_type == LT_OPERATOR)
		return (pa_unit_operator(state, word));
	else
		pa_syntax_error(state, word, "NOT_CAPTURED");
	return (MS_AZ_SUCC);
}
