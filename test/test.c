#include <stdio.h>
#include <stdlib.h>
#include <libc.h>
#include <readline/readline.h>
#include <readline/history.h>

// gcc -lreadline

void	solve(char *s)
{
	int	i = 0;

	while (s[i])
	{
		printf("%c\n", s[i]);
		i++;
	}
	printf("\n");
}


int main()
{
	char *line;
	while (line = readline("> "))
	{
//		printf("line is '%s'\n", line);
		solve(line);
		if (strlen(line) > 0)
			add_history(line);
		free(line);
	}
	printf("\nexit\n");
	return 0;
}
