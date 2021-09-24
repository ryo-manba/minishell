/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 20:06:13 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/25 01:17:21 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_analyzer.h"
#include <unistd.h>
#include <stdio.h>
#define SW 2

void	print_stree(t_master *master, t_stree *stree, int depth)
{
	if (!stree)
		return ;
	printf("%*s[%s", depth * SW, "", pa_token_label(master, stree->token_id));
	if (stree->subshell)
	{
		printf("%.*s", !!SW, "\n");
		print_pipeline(master, stree->subshell, depth + 1);
		printf("%*s] ", depth * SW, "");
	}
	else if (stree->quote_involved)
		printf("{%s}%s] ", stree->token, "q");
	else
		printf("{%s}%s] ", stree->token, "");
	if (stree->right)
		print_stree(master, stree->right, 0);
}

void	print_redir(t_master *master, t_redir *redir, int depth)
{
	const char	*str;
	char		*temp;

	if (!redir)
		return ;
	str = pa_token_label(master, redir->redir_op);
	temp = "";
	if (redir->operand_left)
		temp = redir->operand_left->token;
	printf("%*s(Redir %s%s ",
		depth * SW, "",
		temp,
		str);
	temp = NULL;
	if (redir->operand_right)
		temp = redir->operand_right->token;
	printf("{%s}%.*s) ",
		temp,
		!!redir->operand_right->quote_involved,
		"q"
		);
	if (redir->next)
		print_redir(master, redir->next, 0);
}

void	print_clause(t_master *master, t_clause *clause, int depth)
{
	if (!clause)
		return ;
	printf("%*s{Clause:%.*s", depth * SW, "", !!SW, "\n");
	if (clause->redir)
	{
		print_redir(master, clause->redir, depth + 1);
		printf("%.*s", !!SW, "\n");
	}
	if (clause->stree)
	{
		print_stree(master, clause->stree, depth + 1);
		printf("%.*s", !!SW, "\n");
	}
	printf("%*s}%.*s", depth * SW, "", !!SW, "\n");
	if (clause->next)
		print_clause(master, clause->next, depth);
}

void	print_pipeline(t_master *master, t_pipeline *pipeline, int depth)
{
	const char	*str;
	char		*temp;

	if (!pipeline)
		return ;
	printf("%*s{Pipeline:%.*s", depth * SW, "", !!SW, "\n");
	str = pa_operator_label(master, pipeline->joint);
	print_clause(master, pipeline->clause, depth + 1);
	temp = "";
	if (str)
		temp = (char *)str;
	printf("%*s%s }%.*s", depth * SW, "", temp, !!SW, "\n");
	if (pipeline->next)
		print_pipeline(master, pipeline->next, depth);
}

void	print_words(t_wdlist *words)
{
	char	*lex_type;
	char	*temp_s;
	int		temp_i;

	while (words)
	{
		if (words->lex_type == LT_TOKEN)
			lex_type = "TOKEN";
		if (words->lex_type == LT_IO_NUMBER)
			lex_type = "IO_NUMBER";
		if (words->lex_type == LT_OPERATOR)
			lex_type = "OP";
		temp_s = (char *)words->word;
		temp_i = words->len;
		if (words->lex_type == LT_NEWLINE)
		{
			lex_type = "NEWLINE";
			temp_s = "\\n";
			temp_i = 2;
		}
		printf("<%s{%.*s}'%d'> ", lex_type, temp_i, temp_s, words->delimiter);
		words = words->next;
	}
	printf("\n");
}
