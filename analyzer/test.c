#include "test_analyzer.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

const char	*g_commands_ok[] = {
	// "echo he$VAR w*d wi\"'d'${VAR}e\" '\"$BAR\"'\n",
	// "$VAR@$1VAR_'${VAR\n",
	// "ab\"cd$VAR@@\"ef\n",
	// "$VAR\"a$VAR@\"$var\n",
	// "$VAR\"\"\"${VAR}$~\"${?}\n",
	"cat << EOT << 'EOT' << \"EOT\" << \"\"EOT << E\"O\"T << E${VAR}T << * > E${VAR}T 2>*\n",
	// "export VAR=*\n",
	// "echo a${VAR}\"b   c\"\n",
	// "export a=.*\n",
	// "echo $a\n",
	NULL};

void	print_words(t_wdlist *words)
{
	char *lex_type;

	while (words)
	{
		if (words->lex_type == LT_TOKEN)
			lex_type = "TOKEN";
		if (words->lex_type == LT_IO_NUMBER)
			lex_type = "IO_NUMBER";
		if (words->lex_type == LT_OPERATOR)
			lex_type = "OP";
		if (words->lex_type == LT_NEWLINE)
			lex_type = "NEWLINE";
		printf("<%s{%.*s}> ", lex_type,
			words->lex_type == LT_NEWLINE ? 2 : words->len,
			words->lex_type == LT_NEWLINE ? "\\n" : words->word);
		words = words->next;
	}
	printf("\n");
}

void	print_parse_state(t_parse_state *state)
{
	t_parse_cursor	cursor;

	ft_memcpy(&cursor, &(state->cursor), sizeof(t_parse_cursor));
	printf("[cursor]\npipeline: %p\nclause: %p\nredir: %p\nstree: %p\n",
		cursor.pipeline, cursor.clause, cursor.redir, cursor.stree);
}

void	expand_and_print_pipeline(t_pipeline *pipeline)
{
	t_clause	*clause;
	t_redir		*redir;
	t_redir		*x_redir;
	t_stree		*st;
	t_stree		*x_st;
	t_ex_state	es;
	t_shellvar 	*env;

	env = ms_create_env();
	ms_ex_init_state(&es, env, 0);
	clause = pipeline->clause;
	while (!es.failed && clause)
	{
		print_clause(clause, 0);
		redir = clause->redir;
		while (!es.failed && redir)
		{
			x_redir = ms_expand_a_redir(&es, redir);
			if (!es.failed)
			{
				print_redir(x_redir, 0);
				printf("\n");
			}
			pa_destroy_redir(x_redir);
			if (es.failed)
				break ;
			redir = redir->next;
		}
		if (es.failed)
			break ;
		st = clause->stree;
		x_st = ms_expand_stree(&es, st);
		if (!es.failed)
		{
			print_stree(x_st, 0);
			printf("\n");
		}
		pa_destroy_stree(x_st);
		clause = clause->next;
	}
	ms_env_all_free(env);
	if (!es.failed && pipeline->next)
		expand_and_print_pipeline(pipeline->next);
}

int main()
{
	int 			i;
	t_wdlist		*words;
	t_lex_cursor	cursor;

	t_parse_state	ps;
	// t_ex_state		es;

	setvbuf(stdout, (char *)NULL, _IONBF, 0);
	i = -1;
	while (g_commands_ok[++i])
	{	
		printf("%s\n", g_commands_ok[i]);
		// [Lex]
		words = ms_lexer(g_commands_ok[i], cursor);
		if (words)
			print_words(words);
		else
			continue ;

		// [Parse]
		ms_parse(&ps, words, 0);
		lx_destroy_token(words);
		if (ps.failed)
			continue ;
		else
		{
			print_pipeline(ps.pipeline, 0);
			printf("\n");
		}

		// [Expand]
		expand_and_print_pipeline(ps.pipeline);
		// env = ms_create_env();
		// ms_ex_init_state(&es, env, 0);
		// print_stree(&ps, ps.pipeline->clause->stree, 0);
		// printf("\n");
		// t_stree *expd = ms_expand_stree(&es, ps.pipeline->clause->stree);
		// printf("%s", g_commands_ok[i]);
		// print_stree(&ps, expd, 0);
		// printf("\n");

		// ms_env_all_free(env);
		// env = NULL;
		// pa_destroy_stree(expd);
		pa_destroy_pipeline(ps.pipeline);
	}
	// system("leaks tlex");
}
