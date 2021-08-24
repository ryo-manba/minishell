#include "ms_analyzer.h"

// パーサを初期化
int	ms_init_parse_state(
	t_parse_state *state,
	t_wdlist *words,
	int for_subshell)
{
	ft_bzero(state, sizeof(t_parse_state));
	state->pipeline = ft_calloc(1, sizeof(t_pipeline));
	if (!state->pipeline)
		return (MS_AZ_FAIL);
	printf("%p, %p, %p\n", state, &(state->cursor), state->cursor.word);
	state->cursor.word = words;
	state->for_subshell = for_subshell;
	return (MS_AZ_SUCC);
}

// Word(Lexerトークン)を先頭から1つ取って返す。
// 構造は破壊しない。
// ないならNULLを返す。
t_wdlist	*ms_shift_word(t_parse_state *state)
{
	t_wdlist	*rv;

	if (!state->cursor.word)
		return (NULL);
	rv = state->cursor.word;
	state->cursor.word = rv->next;
	return (rv);
}

int	ms_parse(t_parse_state *state)
{
	int	i;

	i = -1;
	while (++i < 10000)
	{
		if (state->finished)
			break ;
		if (state->error_message)
			break ;
		if (ms_parse_unit(state))
			break ;
	}
	return (MS_AZ_SUCC);
}


