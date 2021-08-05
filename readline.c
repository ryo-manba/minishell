#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

int main()
{
	char *line;
	while (1)
	{
		line = readline("% ");
		printf("line: \"%s\"\n", line);
		add_history(line);
		free(line);
	}
}