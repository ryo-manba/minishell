/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_optable.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 00:38:16 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/25 03:52:07 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

static t_op	*lx_copy_table(t_op ops[])
{
	t_op	*copied;
	size_t	n;
	size_t	i;

	n = 0;
	while (ops[n].label)
		n += 1;
	copied = (t_op *)malloc(sizeof(t_op) * (n + 1));
	if (!copied)
		return (NULL);
	i = 0;
	while (i <= n)
	{
		copied[i] = ops[i];
		i += 1;
	}
	return (copied);
}

int	lx_copy_op_table(t_master *master)
{
	master->lx_ops = lx_copy_table((t_op []){
		{"<<-", TI_LTLTHYPHEN, 3},
		{"<<", TI_LTLT, 2},
		{">>", TI_GTGT, 2},
		{"<&", TI_LTAND, 2},
		{">&", TI_GTAND, 2},
		{"<>", TI_LTGT, 2},
		{"&&", TI_ANDAND, 2},
		{"||", TI_PIPEPIPE, 2},
		{";;", TI_DSEMICOLON, 2},
		{"<", TI_LT, 1},
		{">", TI_GT, 1},
		{"|", TI_PIPE, 1},
		{"&", TI_AND, 1},
		{";", TI_SEMICOLON, 1},
		{"(", TI_PAREN_L, 1},
		{")", TI_PAREN_R, 1},
		{0},
		});
	return (!master->lx_ops);
}

int	lx_copy_all_op_table(t_master *master)
{
	master->lx_all_ops = lx_copy_table((t_op []){
		{"WORD", TI_WORD, 0},
		{"IO_NUMBER", TI_IO_NUMBER, 0},
		{"NAME", TI_NAME, 0},
		{"ASSIGNMENT_WORD", TI_ASSIGNMENT_WORD, 0},
		{"SUBSHELL", TI_SUBSHELL, 0},
		{"<", TI_LT, 0},
		{">", TI_GT, 0},
		{"<<", TI_LTLT, 0},
		{">>", TI_GTGT, 0},
		{"<>", TI_LTGT, 0},
		{"<&", TI_LTAND, 0},
		{">&", TI_GTAND, 0},
		{"<<-", TI_LTLTHYPHEN, 0},
		{"|", TI_PIPE, 0},
		{"&&", TI_ANDAND, 0},
		{"||", TI_PIPEPIPE, 0},
		{"&", TI_AND, 0},
		{";", TI_SEMICOLON, 0},
		{";;", TI_DSEMICOLON, 0},
		{"(", TI_PAREN_L, 0},
		{")", TI_PAREN_R, 0},
		{"NONE", TI_NONE, 0},
		{0}});
	return (!master->lx_all_ops);
}
