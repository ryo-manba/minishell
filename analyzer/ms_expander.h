#ifndef MS_EXPANDER_H
# define MS_EXPANDER_H
# include "../libft/libft.h"
# include "ms_parser.h"

typedef struct s_expander_state
{
	t_shellvar	*var;
	int			last_exit_status;
	t_pipeline	*pipeline;
}	t_expander_state;

int	ms_expand_redir(t_expander_state *state, t_redir **redir);
int	ms_expand_stree(t_expander_state *state, t_stree **stree);

#endif