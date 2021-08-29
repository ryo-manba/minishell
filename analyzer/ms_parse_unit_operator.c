#include "ms_analyzer.h"

// サブシェルのパース開始
int	pa_sub_enter_subshell(t_parse_state *state, t_wdlist *word)
{
	t_parse_state	substate;
	t_stree			*st;

	if (!state->cursor.word->next)
		return (pa_syntax_error(state, word, "NO_RIGHT_ELEM"));
	if (state->cursor.stree)
		return (pa_syntax_error(state, word, "SUBSHELL_IS_NOT_LEADING"));
	if (state->cursor.redir)
		return (pa_syntax_error(state, word, "REDIR_BEFORE_SUBSHELL"));
	if (ms_parse(&substate, word->next, 1))
		return (pa_syntax_error(state, word, "ALLOCATION FAILED"));
	if (substate.err_message && substate.err_word)
		return (pa_syntax_error(state,
				substate.err_word, substate.err_message));
	state->cursor.word = substate.cursor.word;
	st = (t_stree *)ft_calloc(1, sizeof(t_stree));
	if (!st)
		return (pa_syntax_error(state, word, "ALLOCATION FAILED"));
	st->token_id = TI_SUBSHELL;
	st->subshell = substate.pipeline;
	if (!pa_add_stree(state, st))
		return (pa_syntax_error(state, word, "ALLOCATION FAILED"));
	return (MS_AZ_SUCC);
}

// サブシェルのパース終了
int	pa_sub_leave_subshell(t_parse_state *state, t_wdlist *word)
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

// パイプライン終端トークン(; & || &&)
int	pa_sub_term_pipeline(t_parse_state *state, t_wdlist *word)
{
	t_token_id	joint;

	if (pa_syntax_term_pipeline(state, false))
		return (MS_AZ_FAIL);
	joint = pa_operator_token_id(word);
	state->cursor.pipeline->joint = joint;
	if (!pa_add_new_pipeline(state))
		return (pa_syntax_error(state, word, "PIPELINE ALLOCATION FAILED"));
	state->cursor.expecting_continuation
		= (joint == TI_ANDAND || joint == TI_PIPEPIPE);
	return (MS_AZ_SUCC);
}

// 節終端トークン(|)
int	pa_sub_term_clause(t_parse_state *state, t_wdlist *word)
{
	t_token_id	term;

	if (pa_syntax_term_clause(state, 0))
		return (MS_AZ_FAIL);
	if (!pa_add_new_clause(state))
		return (pa_syntax_error(state, word, "CLAUSE ALLOCATION FAILED"));
	term = pa_operator_token_id(word);
	state->cursor.expecting_continuation = (term == TI_PIPE);
	return (MS_AZ_SUCC);
}
