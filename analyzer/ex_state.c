#include "ms_analyzer.h"

void	ms_ex_init_state(t_ex_state *state, t_shellvar *env,
		int last_exit_status)
{
	ft_bzero(state, sizeof(t_ex_state));
	state->var = env;
	state->last_exit_status = last_exit_status;
}

int		ex_mark_failed(t_ex_state *state, int mark)
{
	if (mark)
	{
		state->failed = 1;
		if (!state->error_printed)
		{
			ft_putstr_fd("-" MS_AZ_PROGNAME ": error in expansion\n",
					STDERR_FILENO);
			state->error_printed = 1;
		}
	}
	return (state->failed);
}
