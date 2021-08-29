#include "ms_analyzer.h"

static void	destroy_pipeline(t_pipeline *pipeline);

// パーサを初期化
static int	init_parser(t_parse_state *state, t_wdlist *words, int for_subshell)
{
	ft_bzero(state, sizeof(t_parse_state));
	state->pipeline = ft_calloc(1, sizeof(t_pipeline));
	if (!state->pipeline)
		return (MS_AZ_FAIL);
	state->cursor.word = words;
	state->for_subshell = for_subshell;
	return (MS_AZ_SUCC);
}

static void	destroy_clause(t_clause *clause)
{
	t_redir		*redir;
	t_stree		*stree;
	void		*temp;

	redir = clause->redir;
	while (redir)
	{
		temp = redir->next;
		free(redir);
		redir = temp;
	}
	stree = clause->stree;
	while (stree)
	{
		temp = stree->right;
		free(stree->token);
		destroy_pipeline(stree->subshell);
		free(stree);
		stree = temp;
	}
}

static void	destroy_pipeline(t_pipeline *pipeline)
{
	t_clause	*clause;
	void		*temp;

	while (pipeline)
	{
		clause = pipeline->clause;
		while (clause)
		{
			temp = clause->next;
			destroy_clause(clause);
			clause = temp;
		}
		temp = pipeline->next;
		free(pipeline);
		pipeline = temp;
	}
}

int	ms_parse(t_parse_state *state, t_wdlist *words, int for_subshell)
{
	int	i;

	if (init_parser(state, words, for_subshell))
		return (MS_AZ_FAIL);
	i = -1;
	while (++i < 10000)
	{
		if (state->finished)
			break ;
		if (state->err_message)
			break ;
		if (pa_unit(state))
			break ;
	}
	if (state->err_message)
	{
		destroy_pipeline(state->pipeline);
		state->pipeline = NULL;
	}
	return (MS_AZ_SUCC);
}
