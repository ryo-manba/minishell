/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pa_destroy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:19:54 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/06 00:20:02 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

void	pa_destroy_stree(t_stree *stree)
{
	t_stree	*left;
	t_stree	*right;

	if (!stree)
		return ;
	free(stree->token);
	left = stree->left;
	right = stree->right;
	free(stree);
	if (left)
		pa_destroy_stree(left);
	if (right)
		pa_destroy_stree(right);
}

void	pa_destroy_redir(t_redir *redir)
{
	t_stree	*left;
	t_stree	*right;
	t_redir	*next;

	if (!redir)
		return ;
	left = redir->operand_left;
	right = redir->operand_right;
	next = redir->next;
	free(redir);
	if (left)
		pa_destroy_stree(left);
	if (right)
		pa_destroy_stree(right);
	if (next)
		pa_destroy_redir(next);
}

void	pa_destroy_clause(t_clause *clause)
{
	t_stree		*stree;
	t_redir		*redir;
	t_clause	*next;

	if (!clause)
		return ;
	stree = clause->stree;
	redir = clause->redir;
	next = clause->next;
	free(clause);
	if (stree)
		pa_destroy_stree(stree);
	if (redir)
		pa_destroy_redir(redir);
	if (next)
		pa_destroy_clause(next);
}

void	pa_destroy_pipeline(t_pipeline *pipeline)
{
	t_clause	*clause;
	t_pipeline	*next;

	if (!pipeline)
		return ;
	clause = pipeline->clause;
	next = pipeline->next;
	free(pipeline);
	if (clause)
		pa_destroy_clause(clause);
	if (next)
		pa_destroy_pipeline(next);
}
