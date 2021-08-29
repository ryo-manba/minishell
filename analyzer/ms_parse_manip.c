#include "ms_analyzer.h"

// Word(Lexerトークン)を先頭から1つ取って返す。
// 構造は破壊しない。
// ないならNULLを返す。
t_wdlist	*pa_shift_word(t_parse_state *state)
{
	t_wdlist	*rv;

	if (!state->cursor.word)
		return (NULL);
	rv = state->cursor.word;
	state->cursor.word = rv->next;
	return (rv);
}

// 新しいpipelineをアクティブなpipelineとして追加する。
// 成功したら、新しいpipelineを返す。
t_pipeline	*pa_add_new_pipeline(t_parse_state *state)
{
	t_pipeline	*pipeline;

	pipeline = (t_pipeline *)ft_calloc(1, sizeof(t_pipeline));
	if (!pipeline)
		return (NULL);
	if (state->cursor.pipeline)
		state->cursor.pipeline->next = pipeline;
	else
		state->pipeline = pipeline;
	state->cursor.pipeline = pipeline;
	state->cursor.clause = NULL;
	state->cursor.redir = NULL;
	state->cursor.stree = NULL;
	printf("added new pipeline\n");
	return (pipeline);
}

// 新しいclauseをアクティブなclauseとして追加する。
// 必要ならより上位の構造も追加する。
// 成功したら、新しいclauseを返す。
t_clause	*pa_add_new_clause(t_parse_state *state)
{
	t_clause	*clause;

	if (!state->cursor.pipeline)
		if (!pa_add_new_pipeline(state))
			return (NULL);
	clause = (t_clause *)ft_calloc(1, sizeof(t_clause));
	if (!clause)
		return (NULL);
	if (state->cursor.clause)
		state->cursor.clause->next = clause;
	else
		state->cursor.pipeline->clause = clause;
	state->cursor.clause = clause;
	state->cursor.redir = NULL;
	state->cursor.stree = NULL;
	printf("added new clause\n");
	return (clause);
}

// redirを現在アクティブなclauseに追加する。
// 必要ならより上位の構造も追加する。
// 成功したら、redir自身を返す。
t_redir	*pa_add_redir(t_parse_state *state, t_redir *redir)
{
	if (!state->cursor.clause)
		if (!pa_add_new_clause(state))
			return (NULL);
	if (state->cursor.redir)
		state->cursor.redir->next = redir;
	else
		state->cursor.clause->redir = redir;
	state->cursor.redir = redir;
	state->cursor.expecting_continuation = 0;
	printf("added new redir\n");
	return (redir);
}

// streeを現在アクティブなclauseに追加する。
// 必要ならより上位の構造も追加する。
// 成功したら、stree自身を返す。
t_stree	*pa_add_stree(t_parse_state *state, t_stree *stree)
{
	if (!state->cursor.clause)
	{
		if (!pa_add_new_clause(state))
		{
			free(stree);
			return (NULL);
		}
	}
	if (state->cursor.stree)
		state->cursor.stree->right = stree;
	else
		state->cursor.clause->stree = stree;
	state->cursor.stree = stree;
	state->cursor.expecting_continuation = 0;
	printf("added new stree\n");
	return (stree);
}
