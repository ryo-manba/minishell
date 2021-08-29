#include "ms_analyzer.h"

void	ex_init_cursor(t_ex_cursor *cursor, t_token_id tid, const char *str)
{
	ft_bzero(cursor, sizeof(t_ex_cursor));
	cursor->str = str;
	cursor->n = ft_strlen(str);
	cursor->running = XI_BARE;
	cursor->pa_token_id = tid;
}

t_ex_token	*ex_expansion_error(t_ex_state *state,
		t_ex_cursor *cursor)
{
	(void)state;
	ex_destroy_token(cursor->head);
	ft_putstr_fd("unexpected state on expansion stage", STDERR_FILENO);
	return (NULL);
}

t_ex_token	*ex_push_back_token(t_ex_state *state,
		t_ex_cursor *cursor, const char *given_str)
{
	t_ex_token	*ext;

	ext = (t_ex_token *)ft_calloc(1, sizeof(t_ex_token));
	if (!ext)
		return (ex_expansion_error(state, cursor));
	if (cursor->tail)
		cursor->tail->right = ext;
	cursor->tail = ext;
	cursor->pa_token_id = cursor->pa_token_id;
	if (!cursor->head)
		cursor->head = cursor->tail;
	ext->token_id = cursor->running;
	if (given_str)
		ext->token = given_str;
	else
	{
		ext->token = ft_substr(cursor->str, cursor->vs, cursor->i);
		if (!ext->token)
		{
			state->failed = 1;
			return (NULL);
		}
	}
	return (ext);
}

t_ex_token	*ex_shell_param_unit(t_ex_state *state, t_token_id tid,
	const char *token)
{
	t_ex_cursor	csr;

	ex_init_cursor(&csr, tid, token);
	while (csr.i <= csr.n)
	{
		if ((!ex_sp_close_quote(state, &csr)
				&& ex_sp_open_bvar(&csr))
			|| (!ex_sp_close_var(state, &csr)
				&& !ex_sp_close_bare(state, &csr)))
		{
			if (state->failed)
				break ;
			continue ;
		}
		if (!csr.str[csr.i])
			return (csr.head);
		ex_sp_open_any(&csr);
		csr.i += 1;
	}
	return (ex_expansion_error(state, &csr));
}

t_ex_token	*ex_shell_param(t_ex_state *state, t_stree *stree)
{
	t_ex_token	*head;
	t_ex_token	*expanded;
	t_ex_token	*tail;

	head = NULL;
	tail = NULL;
	while (stree)
	{
		expanded = ex_shell_param_unit(state, stree->token_id, stree->token);
		if (!expanded)
			return (NULL);
		if (tail)
			tail->right = expanded;
		tail = expanded;
		if (!head)
			head = tail;
		stree = stree->right;
	}
	return (head);
}
