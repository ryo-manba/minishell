/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pa_manip.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:20:36 by yokawada          #+#    #+#             */
/*   Updated: 2021/10/01 18:45:43 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

t_wdlist	*pa_shift_lx_token(t_parse_state *state)
{
	t_wdlist	*rv;

	if (!state->cursor.word)
		return (NULL);
	rv = state->cursor.word;
	state->cursor.word = rv->next;
	return (rv);
}

t_pipeline	*pa_add_new_pipeline(t_parse_state *state)
{
	t_pipeline	*pipeline;

	pipeline = (t_pipeline *)ft_calloc(1, sizeof(t_pipeline));
	if (!pipeline)
	{
		pa_generic_error(state, NULL, "bad alloc pipeline");
		return (NULL);
	}
	if (state->cursor.pipeline)
		state->cursor.pipeline->next = pipeline;
	if (!state->pipeline)
		state->pipeline = pipeline;
	state->cursor.pipeline = pipeline;
	state->cursor.clause = NULL;
	state->cursor.redir = NULL;
	state->cursor.stree = NULL;
	return (pipeline);
}

t_clause	*pa_add_new_clause(t_parse_state *state)
{
	t_clause	*clause;

	if (!state->cursor.pipeline)
		if (!pa_add_new_pipeline(state))
			return (NULL);
	clause = (t_clause *)ft_calloc(1, sizeof(t_clause));
	if (!clause)
	{
		pa_generic_error(state, NULL, "bad alloc clause");
		return (NULL);
	}
	if (state->cursor.clause)
		state->cursor.clause->next = clause;
	else
		state->cursor.pipeline->clause = clause;
	state->cursor.clause = clause;
	state->cursor.redir = NULL;
	state->cursor.stree = NULL;
	return (clause);
}

t_redir	*pa_add_redir(t_parse_state *state, t_redir *redir)
{
	if (!state->cursor.clause)
	{
		if (!pa_add_new_clause(state))
		{
			pa_destroy_redir(redir);
			return (NULL);
		}
	}
	if (state->cursor.redir)
		state->cursor.redir->next = redir;
	else
		state->cursor.clause->redir = redir;
	state->cursor.redir = redir;
	state->cursor.expecting_continuation = 0;
	return (redir);
}

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
	return (stree);
}
