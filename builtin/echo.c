#include <libc.h>
#include "../libft/libft.h"
#include <stdbool.h>

void	ft_echo(char *option, char *s)
{
	bool	op_flag;

	op_flag = false;
	if (option && !ft_strcmp(option, "-n"))
		op_flag = true;
	printf("%s", s);
	if (!op_flag)
		printf("\n");
}

int	main(int argc, char **argv, char **envp)
{
	char *op = "-n";
	char *s1 = "hello";
	char *s2 = "test";
	char *s3 = "'4'2't'o'k'y'o'";
	char *s4 = "4 2 t o k y o ";
	char *s5 = "H       E          L          L            O";

	ft_echo(op, s1);
	ft_echo(op, s2);
	ft_echo(op, s3);
	ft_echo(op, s4);
	ft_echo(op, s5);

	ft_echo(NULL, s1);
	ft_echo("-a", s2);
	ft_echo("--n", s3);
	ft_echo("-n-", s4);
	ft_echo("n", s5);

	return (0);
}
