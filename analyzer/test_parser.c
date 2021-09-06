#include "test_analyzer.h"
#include <unistd.h>
#include <stdio.h>
#define SW 2

void	print_stree(t_stree *stree, int depth)
{
	if (!stree)
		return ;
	printf("%*s[%s: ", depth*SW, "", pa_token_label(stree->token_id));
	if (stree->subshell)
	{
		printf("%.*s", !!SW, "\n");
		print_pipeline(stree->subshell, depth + 1);
		printf("%*s] ", depth*SW, "");
	}
	else
		printf("%s] ", stree->token);
	if (stree->right)
		print_stree(stree->right, 0);
}

void	print_redir(t_redir *redir, int depth)
{
	const char *str;

	if (!redir)
		return ;
	str = pa_token_label(redir->redir_op);
	printf("%*s(%s%s ",
		depth*SW, "",
		redir->operand_left ? redir->operand_left->token : "",
		str);
	printf("%s) ",
		redir->operand_right ? redir->operand_right->token : NULL);
	if (redir->next)
		print_redir(redir->next, 0);
}

void	print_clause(t_clause *clause, int depth)
{
	if (!clause)
		return ;
	printf("%*s{Clause:%.*s", depth*SW, "", !!SW, "\n");
	if (clause->redir)
	{
		print_redir(clause->redir, depth + 1);
		printf("%.*s", !!SW, "\n");
	}
	if (clause->stree)
	{
		print_stree(clause->stree, depth + 1);
		printf("%.*s", !!SW, "\n");
	}
	printf("%*s}%.*s", depth*SW, "", !!SW, "\n");
	if (clause->next)
		print_clause(clause->next, depth);
}

void	print_pipeline(t_pipeline *pipeline, int depth)
{
	const char *str;

	if (!pipeline)
		return ;
	printf("%*s{Pipeline:%.*s", depth*SW, "", !!SW, "\n");
	str = pa_operator_label(pipeline->joint);
	print_clause(pipeline->clause, depth + 1);
	printf("%*s%s }%.*s", depth*SW, "", str ? str : "", !!SW, "\n");
	if (pipeline->next)
		print_pipeline(pipeline->next, depth);
}
