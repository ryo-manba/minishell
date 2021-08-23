#include "ms_analyzer.h"

int	ms_syntax_term_clause(t_parse_state *state, int by_newline)
{
	t_clause	*clause;

	clause = state->cursor.clause;
	if (!clause)
		return (ms_return_with_error(state, state->cursor.word, "NO CLAUSE"));
	if (!clause->stree && !clause->redir)
	{
		// streeが存在しない
		// NEWLINEによる呼び出しでないならシンタックスエラー
		if (!by_newline)
			return (ms_return_with_error(state, state->cursor.word, "BLANK_CLAUSE"));
		// NEWLINEによる呼び出しであってもシンタックスエラー
		if (state->cursor.expecting_continuation)
			return (ms_return_with_error(state, state->cursor.word, "EXPECTED_CONTINUATION"));
	}
	return (MS_AZ_SUCC);
}

int	ms_syntax_term_pipeline(t_parse_state *state, int by_newline)
{
	if (ms_syntax_term_clause(state, by_newline))
		return (MS_AZ_FAIL);
	return (MS_AZ_SUCC);
}
