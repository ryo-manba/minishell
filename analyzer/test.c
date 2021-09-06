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
	"echo 2>x && echo 3>x\n",
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

int main()
{
	int 			i;
	t_wdlist		*words;
	t_parse_state	ps;
	t_ex_state		es;
	t_shellvar 		*env;

	setvbuf(stdout, (char *)NULL, _IONBF, 0);
	env = ms_create_env();
	i = -1;
	while (g_commands_ok[++i])
	{	
		printf("%s\n", g_commands_ok[i]);
		// [Lex]
		words = ms_lexer(g_commands_ok[i]);
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
			print_pipeline(&ps, ps.pipeline, 0);
			printf("\n");
		}

		// [Expand]
		ft_bzero(&es, sizeof(t_ex_state));
		ms_ex_init_state(&es, env, 0);
		print_stree(&ps, ps.pipeline->clause->stree, 0);
		printf("\n");
		t_stree *expd = ms_expand_stree(&es, ps.pipeline->clause->stree);
		printf("%s", g_commands_ok[i]);
		print_stree(&ps, expd, 0);
		printf("\n");

		pa_destroy_stree(expd);
		pa_destroy_pipeline(ps.pipeline);
	}
	// system("leaks tlex");
}
