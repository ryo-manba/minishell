#include "ms_analyzer.h"

void	ex_ll_open_any(t_ex_state *state, t_ex_unit_cursor *cursor)
{
	int		pos;
	char	c;

	// printf("i = %zu, c = '%c'\n", cursor->i, cursor->str[cursor->i]);
	c = cursor->str[cursor->i];
	pos = ft_strchr_i("'\"$", c);
	if (pos >= 0)
	{
		if (c == '\'') 
		{
			// シングルクオート内処理
			cursor->vs = cursor->i + 1;
			cursor->running = XI_SQUOTED;
			cursor->i += 1;
			while (cursor->str[cursor->i] && cursor->str[cursor->i] != c)
				cursor->i += 1;
			if (!ex_push_back_token(state, cursor, NULL))
				state->failed = 1;
			cursor->i += 1;
			return ;
		}
		else if (c == '"')
		{
			// ダブルクオート内処理
			t_ex_unit_cursor	csr;

			printf("[in deep]\n");
			cursor->i += 1;
			ex_ll_init_cursor(&csr, cursor->pa_token_id,
				cursor->str + cursor->i, '"');
			ex_ll_unit(state, &csr);
			printf("[out deep] err?: %d\n", state->failed);
			ex_stringify_extoken(csr.p.head);
			printf("i -> %zu + %zu\n", cursor->i, csr.i);
			cursor->i += csr.i;
			return ;
		}
		else
		{
			cursor->i += 1;
			cursor->vs = cursor->i;
			cursor->running = XI_VAR;
			return ;
		}
	}
	cursor->vs = cursor->i;
	cursor->running = XI_BARE;
	cursor->i += 1;
}

int	ex_ll_open_bvar(t_ex_unit_cursor *cursor)
{
	char	c;

	c = cursor->str[cursor->i];
	if (cursor->running != XI_VAR || cursor->vs != cursor->i || c != '{')
		return (0);
	cursor->vs = cursor->i + 1;
	cursor->running = XI_BRACED_VAR;
	cursor->i += 1;
	// printf("trapped: open bvar\n");
	return (1);
}
