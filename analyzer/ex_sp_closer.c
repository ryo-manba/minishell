#include "ms_analyzer.h"

int	ex_sp_close_d_quote(t_ex_state *state, t_ex_cursor *cursor)
{
	char		*quoted_str;
	t_ex_token	*inner_ext;
	char		*joined;

	quoted_str = ft_substr(cursor->str, cursor->vs, cursor->i);
	if (!quoted_str)
		return (MS_AZ_FAIL);
	state->ex_quoted = 1;
	inner_ext = ex_shell_param_unit(state, cursor->pa_token_id, quoted_str);
	state->ex_quoted = 0;
	free(quoted_str);
	joined = ex_lstcat(inner_ext);
	ex_destroy_token(inner_ext);
	if (joined)
	{
		if (ex_push_back_token(state, cursor, joined))
			return (MS_AZ_SUCC);
		free(joined);
	}
	return (MS_AZ_FAIL);
}

// SQUOTED, DQUOTEDの終了
// trapped: returns 1;
// untrapped: returns 0;
// failed: returns 1 with state.failed = 1;
int	ex_sp_close_quote(t_ex_state *state, t_ex_cursor *cursor)
{
	char	c;

	c = cursor->str[cursor->i];
	if (cursor->running == XI_SQUOTED && c == '\'')
	{
		if (!ex_push_back_token(state, cursor, NULL))
			state->failed = 1;
	}
	else if (cursor->running == XI_DQUOTED && c == '"')
	{
		if (ex_sp_close_d_quote(state, cursor))
			state->failed = 1;
	}
	else
		return (0);
	cursor->i += 1;
	return (1);
}

// VAR, BRACED_VARの終了
// trapped: returns 1;
// untrapped: returns 0;
// failed: returns 1 with state.failed = 1;
int	ex_sp_close_var(t_ex_state *state, t_ex_cursor *cursor)
{
	char	c;
	int		dec;
	int		inc;

	c = cursor->str[cursor->i];
	if (cursor->running == XI_BRACED_VAR && c == '}')
	{
		dec = 1;
		inc = 2;
	}
	else if (cursor->running == XI_VAR
		&& !lx_is_char_for_name(c, cursor->i - cursor->vs - 1))
	{
		dec = 0;
		inc = 0;
	}
	else
		return (0);
	cursor->i -= dec;
	if (ex_replace_var(state, cursor))
		state->failed = 1;
	cursor->i += inc;
	return (1);
}

// BAREの終了
// trapped: returns 1;
// untrapped: returns 0;
// failed: returns 1 with state.failed = 1;
int	ex_sp_close_bare(t_ex_state *state, t_ex_cursor *cursor)
{
	char	c;

	c = cursor->str[cursor->i];
	if (cursor->running != XI_BARE || lx_is_char_for_bare(c))
		return (0);
	if (cursor->vs < cursor->i)
		if (!ex_push_back_token(state, cursor, NULL))
			state->failed = 1;
	cursor->i += 1;
	return (1);
}
