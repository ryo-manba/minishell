#include "ms_builtin.h"

void	blt_cd_print_error(char *dirname, char *message)
{
	if (dirname == NULL)
	{
		ft_putstr_fd("getcwd: cannot access parent directories", 2);
	}
	else
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(dirname, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(message, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
}
