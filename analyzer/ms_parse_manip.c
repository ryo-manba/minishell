#include "ms_analyzer.h"

// 新しいpipelineをアクティブなpipelineとして追加する。
// 成功したら、新しいpipelineを返す。
t_pipeline	*ms_parse_add_new_pipeline(t_parse_state *state)
{
	t_pipeline	*pipeline;

	pipeline = (t_pipeline *)ft_calloc(1, sizeof(t_pipeline));
	if (pipeline)
	{
		if (state->cursor.pipeline)
			state->cursor.pipeline->next = pipeline;
		else
			state->pipeline = pipeline;
		state->cursor.pipeline = pipeline;
		state->cursor.clause = NULL;
		state->cursor.redir = NULL;
		state->cursor.stree = NULL;
		printf("added new pipeline\n");
	}
	return (pipeline);
}

// 新しいclauseをアクティブなclauseとして追加する。
// 必要ならより上位の構造も追加する。
// 成功したら、新しいclauseを返す。
t_clause	*ms_parse_add_new_clause(t_parse_state *state)
{
	t_clause	*clause;

	// pipelineがなければ空のpipelineを作成
	if (!state->cursor.pipeline)
		if (!ms_parse_add_new_pipeline(state))
			return (NULL);
	clause = (t_clause *)ft_calloc(1, sizeof(t_clause));
	if (clause)
	{
		if (state->cursor.clause)
			state->cursor.clause->next = clause;
		else
			state->cursor.pipeline->clause = clause;
		state->cursor.clause = clause;
		state->cursor.redir = NULL;
		state->cursor.stree = NULL;
		printf("added new clause\n");
	}
	return (clause);
}

// redirを現在アクティブなclauseに追加する。
// 必要ならより上位の構造も追加する。
// 成功したら、redir自身を返す。
t_redir	*ms_parse_add_redir(t_parse_state *state, t_redir *redir)
{
	// clauseがなければ空のclauseを作成
	if (!state->cursor.clause)
		if (!ms_parse_add_new_clause(state))
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
t_stree	*ms_parse_add_stree(t_parse_state *state, t_stree *stree)
{
	// clauseがなければ空のclauseを作成
	if (!state->cursor.clause)
		if (!ms_parse_add_new_clause(state))
			return (NULL);
	if (state->cursor.stree)
		state->cursor.stree->right = stree;
	else
		state->cursor.clause->stree = stree;
	state->cursor.stree = stree;
	state->cursor.expecting_continuation = 0;
	printf("added new stree\n");
	return (stree);
}
