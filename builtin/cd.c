#include "../libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int	ft_cd(char **s)
{
	system("pwd");
	if (s[2] == NULL)
		chdir(getenv("HOME"));
	else
	{
		if (chdir(s[2]) == -1)
			printf("minishell: %s: %s: No such file or directory\n", s[1], s[2]);
	}
	system("pwd");
	return (0);
}

int main(int ac, char **av)
{
	if (ac == 1)
		return (0);
	if (!strcmp(av[1],"cd"))
		ft_cd(av);
	return (0);
}
