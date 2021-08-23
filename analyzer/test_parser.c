#include "test_analyzer.h"
#include <unistd.h>
#include <stdio.h>

void	print_stree(t_parse_state *state, t_stree *stree)
{
	(void)state;
	if (!stree)
		return ;
	printf("[Stree(%s): %s] ", ms_token_label(stree->token_id), stree->token);
	if (stree->right)
		print_stree(state, stree->right);
}

void	print_redir(t_parse_state *state, t_redir *redir)
{
	const char *str;

	(void)state;
	if (!redir)
		return ;
	str = ms_token_label(redir->redir_op);
	printf("[%s%s %s] ",
		redir->operand_left ? redir->operand_left->token : "",
		str,
		redir->operand_right ? redir->operand_right->token : NULL);
	if (redir->next)
		print_redir(state, redir->next);
}

void	print_clause(t_parse_state *state, t_clause *clause)
{
	(void)state;
	if (!clause)
		return ;
	printf("  {Clause:\n");
	if (clause->redir)
	{
		printf("    ");
		print_redir(state, clause->redir);
		printf("\n");
	}
	if (clause->stree)
	{
		printf("    ");
		print_stree(state, clause->stree);
		printf("\n");
	}
	printf("  }\n");
	if (clause->next)
		print_clause(state, clause->next);
}

void	print_pipeline(t_parse_state *state, t_pipeline *pipeline)
{
	const char *str;

	(void)state;
	if (!pipeline)
		return ;
	printf("{Pipeline:\n");
	str = ms_operator_label(pipeline->joint);
	print_clause(state, pipeline->clause);
	printf("%s }\n", str ? str : "");
	if (pipeline->next)
		print_pipeline(state, pipeline->next);
}
