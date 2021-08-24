#include "ms_analyzer.h"

// 改行トークンのパース
int	ms_unit_newline(t_parse_state *state, t_wdlist *word)
{
	char	*final_error;

	if (state->for_subshell)
		return (ms_syntax_error(state, word, "NEWLINE_IN_SUBSHELL"));
	final_error = ms_syntax_final(state);
	if (final_error)
		return (ms_syntax_error(state, word, final_error));
	state->finished = 1;
	return (1);
}

// IO_NUMBERトークンのパース
int	ms_unit_token_io_number(t_parse_state *state, t_wdlist *word)
{
	t_stree		*st;
	t_wdlist	*next_word;

	st = ms_make_stree(word, 0);
	if (!st)
		return (ms_syntax_error(state, word, "ALLOCATION FAILED"));
	next_word = ms_shift_word(state);
	if (!next_word)
		return (ms_syntax_error(state, word, "SOLE_IO_NUMBER"));
	if (ms_subparse_redir(state, next_word, st))
		return (ms_syntax_error(state, next_word, "ALLOCATION FAILED"));
	return (MS_AZ_SUCC);
}

// TOKENトークンのパース
int	ms_unit_token_token(t_parse_state *state, t_wdlist *word)
{
	t_stree	*st;

	st = ms_make_stree(word, 0);
	printf("%p(%.*s) -> %p\n", word, word->len, word->word, st);
	if (!st)
		return (ms_syntax_error(state, word, "ALLOCATION FAILED"));
	if (state->cursor.stree && state->cursor.stree->token_id == TI_SUBSHELL)
		return (ms_syntax_error(state, word, "NEXT_TO_SUBSHELL"));
	if (ms_is_assignment_word(word->word, word->len))
		st->token_id = TI_ASSIGNMENT_WORD;
	if (!ms_parse_add_stree(state, st))
		return (MS_AZ_FAIL);
	return (MS_AZ_SUCC);
}

// OPERATORトークンのパース
int	ms_unit_token_operator(t_parse_state *state, t_wdlist *word)
{
	t_token_id	ti;

	ti = ms_operator_token_id(word);
	if (ti == TI_PAREN_L)
		return (ms_subparse_enter_subshell(state, word));
	if (ti == TI_PAREN_R)
		return (ms_subparse_leave_subshell(state, word));
	if (ti == TI_ANDAND || ti == TI_PIPEPIPE || ti == TI_AND
		|| ti == TI_SEMICOLON)
		return (ms_subparse_term_pipeline(state, word));
	if (ti == TI_PIPE)
		return (ms_subparse_term_clause(state, word));
	if (ti == TI_LT || ti == TI_GT || ti == TI_LTLT || ti == TI_GTGT
		|| ti == TI_LTGT || ti == TI_LTAND || ti == TI_GTAND
		|| ti == TI_LTLTHYPHEN)
		return (ms_subparse_redir(state, word, NULL));
	return (ms_syntax_error(state, word, "NOT_CAPTURED"));
}

// トークンを使い尽くした場合は1を返す。そうでないなら0を返す。
int	ms_parse_unit(t_parse_state *state)
{
	t_wdlist	*word;

	word = ms_shift_word(state);
	if (!word)
		return (1);
	printf("parsing word: %p %.*s\n", word, word->len, word->word);
	if (word->lex_type == LT_NEWLINE)
		return (ms_unit_newline(state, word));
	else if (word->lex_type == LT_IO_NUMBER)
		return (ms_unit_token_io_number(state, word));
	else if (word->lex_type == LT_TOKEN)
		return (ms_unit_token_token(state, word));
	else if (word->lex_type == LT_OPERATOR)
		return (ms_unit_token_operator(state, word));
	else
		ms_syntax_error(state, word, "NOT_CAPTURED");
	return (MS_AZ_SUCC);
}
