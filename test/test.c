#include <stdio.h>
#include <stdlib.h>
#include <libc.h>
#include <readline/readline.h>
#include <readline/history.h>

int main()
{
	char* line;
	while (line = readline("> "))
	{
		printf("line is '%s'\n", line);
		if (strlen(line) > 0) {
		add_history(line);
		}
		free(line);
		rl_replace_line("aaa", 0);
	}
	printf("\nexit\n");
	return 0;
}
