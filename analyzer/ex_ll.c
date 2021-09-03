#include "ms_analyzer.h"

t_ex_token	*ex_expansion_error(t_ex_state *state,
		t_ex_unit_cursor *cursor)
{
	(void)state;
	if (cursor->p.head)
		ex_destroy_token(cursor->p.head);
	ft_putstr_fd("unexpected state on expansion stage\n", STDERR_FILENO);
	return (NULL);
}

t_ex_token	*ex_push_back_token(t_ex_state *state,
		t_ex_unit_cursor *cursor, const char *given_str)
{
	t_ex_token	*ext;

	ext = (t_ex_token *)ft_calloc(1, sizeof(t_ex_token));
	if (!ext)
		return (ex_expansion_error(state, cursor));
	ex_add_token_csr(&(cursor->p), ext);
	ext->pa_token_id = cursor->pa_token_id;
	ext->token_id = cursor->running;
	if (cursor->running == XI_DIVIDER)
		ext->token = NULL;
	else
	{
		ext->token = given_str;
		if (!ext->token)
			ext->token = ft_substr(cursor->str, cursor->substr_s,
				cursor->substr_e - cursor->substr_s);
		if (!ext->token)
		{
			free(ext);
			state->failed = 1;
			return (NULL);
		}
		printf("\"%s\" %zu:%zu\n", ext->token, cursor->substr_s, cursor->substr_e - cursor->substr_s);
	}
	return (ext);
}

// 唯一何かを開始できる and 展開を終了できるモード
int	ex_ll_trap_neutral(t_ex_state *state, t_ex_unit_cursor *csr)
{
	(void)state;
	if (csr->running != XI_NEUTRAL)
		return (0);
	if (csr->str[csr->i] == '\'' && !csr->quote)
	{
		csr->vs = csr->i++;
		csr->running = XI_SQUOTED;
	}
	else if (csr->str[csr->i] == '"')
	{
		if (ex_ll_trap_dquote(state, csr))
			state->failed = 1;
	}
	else if (csr->str[csr->i] == '$')
	{
		csr->vs = csr->i++;
		csr->running = XI_VAR;
	}
	else
	{
		csr->vs = csr->i;
		csr->running = XI_BARE;
	}
	return (1);
}

void	ex_ll_unit(t_ex_state *state, t_ex_unit_cursor *csr)
{
	printf("ex_ll_unit: \"%s\"\n", csr->str);
	while (!state->failed)
	{
		if (csr->running == XI_NEUTRAL && csr->str[csr->i] == csr->quote)
			break ;
		if (ex_ll_trap_neutral(state, csr))
			continue ;
		if (ex_ll_trap_squoted(state, csr))
			continue ;
		if (csr->running == XI_VAR && ex_ll_trap_var(state, csr))
			continue ;
		if (ex_ll_trap_braced_var(state, csr))
			continue ;
		if (ex_ll_trap_bare(state, csr))
			continue ;
	}
	if (state->failed)
		printf("!!FAILED!!\n");
	else
		printf("finished\n");
	return ;
}

t_ex_token	*ex_shell_param(t_ex_state *state, t_stree *stree)
{
	t_ex_unit_cursor	csr;

	ex_ll_init_cursor(&csr, stree->token_id, stree->token, '\0');
	ex_ll_unit(state, &csr);
	return (csr.p.head);
}
