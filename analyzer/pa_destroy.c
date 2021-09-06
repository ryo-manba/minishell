/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pa_destroy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:19:54 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/06 14:11:50 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

void	pa_destroy_stree(t_stree *stree)
{
	t_stree	*temp;

	while (stree)
	{
		free(stree->token);
		pa_destroy_stree(stree->left);
		temp = stree->right;
		free(stree);
		stree = temp;
	}
}

void	pa_destroy_redir(t_redir *redir)
{
	t_redir	*temp;

	while (redir)
	{
		temp = redir->next;
		pa_destroy_stree(redir->operand_left);
		pa_destroy_stree(redir->operand_right);
		free(redir);
		redir = temp;
	}
}

void	pa_destroy_clause(t_clause *clause)
{
	t_clause	*temp;

	while (clause)
	{
		temp = clause->next;
		pa_destroy_stree(clause->stree);
		pa_destroy_redir(clause->redir);
		free(clause);
		clause = temp;
	}
}

void	pa_destroy_pipeline(t_pipeline *pipeline)
{
	t_pipeline	*temp;

	while (pipeline)
	{
		temp = pipeline->next;
		pa_destroy_clause(pipeline->clause);
		free(pipeline);
		pipeline = temp;
	}
}
