#include "ms_analyzer.h"

void	ex_sp_open_any(t_ex_cursor *cursor)
{
	int	pos;

	pos = ft_strchr_i("'\"$", cursor->str[cursor->i]);
	if (pos >= 0)
	{
		cursor->vs = cursor->i + 1;
		cursor->running = (t_ex_token_id[3]){
			XI_SQUOTED, XI_DQUOTED, XI_VAR}[pos];
	}
	else
	{
		cursor->vs = cursor->i;
		cursor->running = XI_BARE;
	}
}

int	ex_sp_open_bvar(t_ex_cursor *cursor)
{
	char	c;

	c = cursor->str[cursor->i];
	if (cursor->running != XI_VAR || cursor->vs != cursor->i || c != '{')
		return (0);
	cursor->vs = cursor->i + 1;
	cursor->running = XI_BRACED_VAR;
	cursor->i += 1;
	return (1);
}
