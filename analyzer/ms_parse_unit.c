#include "ms_analyzer.h"

char *ms_parse_check_final(t_parse_state *state) {
	if (!state->cursor.clause->stree)
		if (state->cursor.expecting_continuation)
			return ("EXPECTED_CONTINUATION");
	return (NULL);
}

// 改行トークンのパース
int	ms_unit_newline(t_parse_state *state, t_wdlist *word)
{
	char	*final_error;

	// サブシェル内だった場合はエラー
	if (state->for_subshell)
		return (ms_return_with_error(state, word, "NEWLINE_IN_SUBSHELL"));
	final_error = ms_parse_check_final(state);
	if (final_error)
		return (ms_return_with_error(state, word, final_error));
	// パース終了処理を行う。
	state->finished = 1;
	// エラーチェック
	return (1);
}

// IO_NUMBERトークンのパース
int	ms_unit_token_io_number(t_parse_state *state, t_wdlist *word)
{
	t_stree		*st;
	t_wdlist	*next_word;

	st = ms_make_stree(word, 0);
	if (!st)
		return (ms_return_with_error(state, word, "ALLOCATION FAILED"));
	// [トークンがIO_NUMBERだった場合]
	// リダイレクションの記述として処理を進める
	next_word = ms_shift_word(state);
	if (!next_word)
		return (ms_return_with_error(state, word, "SOLE_IO_NUMBER")); // シンタックスエラー
	if (ms_subparse_redir(state, next_word, st))
		return (ms_return_with_error(state, next_word, "ALLOCATION FAILED"));
	return (MS_AZ_SUCC);
}

// TOKENトークンのパース
int	ms_unit_token_token(t_parse_state *state, t_wdlist *word)
{
	t_stree	*st;

	st = ms_make_stree(word, 0);
	printf("%p(%.*s) -> %p\n", word, word->len, word->word, st);
	if (!st)
		return (ms_return_with_error(state, word, "ALLOCATION FAILED"));
	// [先行するトークンがサブシェルだった場合]
	// -> エラー
	if (state->cursor.stree && state->cursor.stree->token_id == TI_SUBSHELL)
		return (ms_return_with_error(state, word, "NEXT_TO_SUBSHELL"));

	// [トークンがTOKENだった場合]
	// トークンがASSIGNMENT_WORDであるかどうかをチェックする。
	// TODO: これは対応しないので、パースエラーにする予定
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
	char		*final_error;

	ti = ms_operator_token_id(word);
	// [トークンがなんらかの演算子だった場合]
	// [サブシェルを開始する演算子だった場合]
	// ここから新たなサブシェルパースを開始する
	// ParseStateはまっさらな状態で開始するが、 state->cursor.lexer_token だけは引き継がれる。
	if (ti == TI_PAREN_L)
		return (ms_subparse_subshell(state, word));
	if (ti == TI_PAREN_R)
	{
		// [サブシェルを終了する演算子だった場合]
		// パースを終了する
		// サブシェル解析中でないのにサブシェル終了演算子に遭遇した場合はエラー
		if (!state->for_subshell)
			return (ms_return_with_error(state, word, "UNEXPECTED_SUBSHELL_CLOSER"));
		// サブシェルの中身のトークンが空の場合はエラーとする。
		if (!state->cursor.redir && !state->cursor.stree)
			return (ms_return_with_error(state, word, "UNEXPECTED_SUBSHELL_CLOSER"));
		final_error = ms_parse_check_final(state);
		if (final_error)
			return (ms_return_with_error(state, word, final_error));
		// TODO: NEWLINEによる終了とは異なり、state->cursor.lexer_token が残っていてもOKとすることに注意。
		state->finished = 1;
		// エラーチェック
		return (1);
	}
	// [Pipelineを区切る演算子だった場合]
	// パイプライン終端処理を行う。
	if (ti == TI_ANDAND || ti == TI_PIPEPIPE || ti == TI_AND || ti == TI_SEMICOLON)
		return (ms_subparse_term_pipeline(state, word));
	// [Clauseを区切る演算子だった場合]
	// 節終端処理を行う。
	if (ti == TI_PIPE)
		return (ms_subparse_term_clause(state, word));
	// [リダイレクション演算子だった場合]
	if (ti == TI_LT || ti == TI_GT || ti == TI_LTLT || ti == TI_GTGT ||
		ti == TI_LTGT || ti == TI_LTAND || ti == TI_GTAND ||
		ti == TI_LTLTHYPHEN)
		return (ms_subparse_redir(state, word, NULL));
	return (ms_return_with_error(state, word, "NOT_CAPTURED"));
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
		return (ms_unit_newline(state, word)); // [トークンがNEWLINEだった場合]
	else if (word->lex_type == LT_IO_NUMBER)
		return (ms_unit_token_io_number(state, word)); // [トークンがIO_NUMBERだった場合]
	else if (word->lex_type == LT_TOKEN)
		return (ms_unit_token_token(state, word)); // [トークンがTOKENだった場合]
	else if (word->lex_type == LT_OPERATOR)
		return (ms_unit_token_operator(state, word)); // [トークンがなんらかの演算子だった場合]
	else
		ms_return_with_error(state, word, "NOT_CAPTURED");
	return (MS_AZ_SUCC);
}
