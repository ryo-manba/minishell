#include "test_analyzer.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

const char	*g_commands_ok[] = {
	// "echo \"$VAR\" && echo * > *.md | > README* echo a${VAR}\"$VAR\"'$VAR'\n",
	// "(VAR=\"Hello\" echo a; echo b) | cat > file\n",
	// "(a && (b || (c && (d || e))))\n",
	// "echo \"\" a bb ccc\"\"\n",
	// "a 1>&2\n",
	// "()",
	// "(echo x)",
	"export TEST=test aaa\n",
	// "cmd << HEREDOC arg1 arg2\n",
	// "<<-EOT<>x echo a>>y<< z&&var=phi cat x<<x 1>&y 2>&z&&ls -l\n",
	// "echo hello\n",
	// "echo hello > out.txt\n",
	// "echo hello 0> out.txt\n",
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
		printf("{ type: %s, \"%.*s\" } ", lex_type,
			words->lex_type == 4 ? 2 : words->len,
			words->lex_type == 4 ? "\\n" : words->word);
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
	int 		i;
	t_wdlist	*words;
	t_parse_state	ps;

	setvbuf(stdout, (char *)NULL, _IONBF, 0);
	i = -1;
	while (g_commands_ok[++i])
	{	
		printf("%s\n", g_commands_ok[i]);
		words = ms_lexer(g_commands_ok[i]);
		print_words(words);
		ms_parse(&ps, words, 0);
		printf("%s\n", g_commands_ok[i]);
		print_pipeline(&ps, ps.pipeline, 0);
		printf("\n");
		if (ps.err_message)
			printf("[Parse Error] %s\n", ps.err_message);
	}
}
