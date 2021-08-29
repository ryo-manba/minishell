#include "ms_expander.h"

void		*ex_error(t_ex_state *state, t_stree *stree, char *message)
{
	(void)state;
	(void)stree;

	ft_putstr_fd(message, STDERR_FILENO);
	return (NULL);
}

void		*ex_fatal(t_ex_state *state, char *message)
{
	(void)state;

	ft_putstr_fd(message, STDERR_FILENO);
	return (NULL);
}
