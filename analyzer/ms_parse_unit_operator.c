#include "ms_analyzer.h"

// サブシェルのパース開始
int	ms_subparse_enter_subshell(t_parse_state *state, t_wdlist *word)
{
	t_parse_state	substate;
	t_stree			*st;

	if (!state->cursor.word->next)
		return (ms_return_with_error(state, word, "NO_RIGHT_ELEM"));
	if (state->cursor.stree)
		return (ms_return_with_error(state, word, "SUBSHELL_IS_NOT_LEADING"));
	if (state->cursor.redir)
		return (ms_return_with_error(state, word, "REDIR_BEFORE_SUBSHELL"));
	st = (t_stree *)ft_calloc(1, sizeof(t_stree));
	if (!st)
		return (ms_return_with_error(state, word, "ALLOCATION FAILED"));
	if (ms_init_parse_state(&substate, word->next, 1))
		return (ms_return_with_error(state, word, "ALLOCATION FAILED"));
	ms_parse(&substate);
	if (substate.error_message && substate.error_word)
		return (ms_return_with_error(state, substate.error_word, substate.error_message));
	state->cursor.word = substate.cursor.word;
	st->token_id = TI_SUBSHELL;
	st->subshell = substate.pipeline;
	if (!ms_parse_add_stree(state, st))
		return (ms_return_with_error(state, word, "ALLOCATION FAILED"));
	return (MS_AZ_SUCC);
}

// サブシェルのパース終了
int	ms_subparse_leave_subshell(t_parse_state *state, t_wdlist *word)
{
	char		*final_error;

	if (!state->for_subshell)
		return (ms_return_with_error(state, word, "UNEXPECTED_SUBSHELL_CLOSER"));
	if (!state->cursor.redir && !state->cursor.stree)
		return (ms_return_with_error(state, word, "UNEXPECTED_SUBSHELL_CLOSER"));
	final_error = ms_syntax_final(state);
	if (final_error)
		return (ms_return_with_error(state, word, final_error));
	state->finished = 1;
	return (MS_AZ_SUCC);
}

// パイプライン終端トークン(; & || &&)
int ms_subparse_term_pipeline(t_parse_state *state, t_wdlist *word)
{
	t_token_id	joint;

	if (ms_syntax_term_pipeline(state, false))
		return (MS_AZ_FAIL);
	joint = ms_operator_token_id(word);
	state->cursor.pipeline->joint = joint;
	if (!ms_parse_add_new_pipeline(state))
		return (ms_return_with_error(state, word, "PIPELINE ALLOCATION FAILED"));
	state->cursor.expecting_continuation = (joint == TI_ANDAND || joint == TI_PIPEPIPE);
	return (MS_AZ_SUCC);
}

// 節終端トークン(|)
int ms_subparse_term_clause(t_parse_state *state, t_wdlist *word)
{
	t_token_id	term;

	if (ms_syntax_term_clause(state, 0))
		return (MS_AZ_FAIL);
	if (!ms_parse_add_new_clause(state))
		return (ms_return_with_error(state, word, "CLAUSE ALLOCATION FAILED"));
	term = ms_operator_token_id(word);
	state->cursor.expecting_continuation = (term == TI_PIPE);
	return (MS_AZ_SUCC);
}
