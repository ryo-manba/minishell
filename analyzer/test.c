#include "ms_analyzer.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

const char	*g_commands_ok[] = {
	// "echo \"$VAR\" && echo * > *.md | > README* echo a${VAR}\"$VAR\"'$VAR'\n",
	"\"$VAR\"$VAR\"$VAR\"\n",
	// "echo\n",
	// "echo hello\n",
	// "echo hello > out.txt\n",
	// "echo hello 0> out.txt\n",
	// "echo hello >> out.txt\n",
	NULL};

void	print_words(t_wdlist *words)
{
	while (words)
	{
		// printf("%p\n", words);
		printf("{ type: %d, \"%s\" } ", words->lex_type, words->lex_type == 4 ? "\\n" : words->word);
		words = words->next;
	}
	printf("\n");
}

int main()
{
	int 		i;
	t_wdlist	*words;

	setvbuf(stdout, (char *)NULL, _IONBF, 0);
	i = -1;
	while (g_commands_ok[++i])
	{	
		printf("%s\n", g_commands_ok[i]);
		words = ms_lexer(g_commands_ok[i]);
		print_words(words);
	}
}
