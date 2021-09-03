#include "ms_analyzer.h"

// tail(csr->p.src_tail)を空白で区切りで分割する
int	ex_sp_hash_tail(t_ex_state *state, t_ex_cursor *csr)
{
	int		is_ws;

	csr->i = 0;
	csr->str = csr->p.src_tail->token;
	csr->pa_token_id = csr->p.src_tail->pa_token_id;
	while (csr->str[csr->i])
	{
		csr->vs = csr->i;
		is_ws = !!ft_strchr(EX_IFS, csr->str[csr->i]);
		if (is_ws)
			csr->running = XI_WHT_SPACE;
		else
			csr->running = csr->p.src_tail->token_id;
		while (csr->str[csr->i]
				&& is_ws == !!ft_strchr(EX_IFS, csr->str[csr->i]))
			csr->i += 1;
		if (!ex_push_back_token(state, csr, NULL))
			return (MS_AZ_FAIL);
	}
	return (MS_AZ_SUCC);
}

// 1. Variableサブトークンを空白・非空白に分割
int	ex_split_variable(t_ex_state *state, t_ex_cursor *cursor)
{
	while (cursor->p.src_tail)
	{
		if (cursor->p.src_tail->token_id != XI_VAR)
		{
			ex_add_token_csr(&(cursor->p), cursor->p.src_tail);
			ex_pop_src_token_csr(&(cursor->p));
			continue ;
		}
		if (!ex_sp_hash_tail(state, cursor))
			return (MS_AZ_FAIL);
		ex_pop_src_token_csr(&(cursor->p));
	}
	return (MS_AZ_SUCC);
}

// 3. WS+の並びでDividerを入れる
int	ex_insert_splitter_variable(t_ex_state *state, t_ex_cursor *cursor)
{
	t_ex_token	*copied;

	while (cursor->p.src_tail)
	{
		if (!state->no_split
			&& cursor->p.src_tail->token_id == XI_WHT_SPACE)
		{
			if (cursor->p.src_head != cursor->p.src_tail
				&& cursor->p.src_tail->right)
			{
				cursor->running = XI_SPLITTER;
				if (!ex_push_back_token(state, cursor, NULL))
					return (MS_AZ_FAIL);
				continue;
			}
		}
		copied = (t_ex_token *)ft_calloc(1, sizeof(t_ex_token));
		if (!copied)
			return (MS_AZ_FAIL);
		ft_memcpy(copied, cursor->p.src_tail, sizeof(t_ex_token));
		copied->right = NULL;
		ex_add_token_csr(&(cursor->p), copied);
		ex_pop_src_token_csr(&(cursor->p));
	}
	return (MS_AZ_SUCC);
}


t_ex_token	*ex_split_word(t_ex_state *state, t_ex_token *token)
{
	t_ex_cursor	csr;

	ex_init_sw_cursor(&csr, token);
	if (ex_split_variable(state, &csr))
	{
		ex_destroy_token(csr.p.head);
		return (NULL);
	}
	csr.p.src_head = csr.p.head;
	csr.p.src_tail = csr.p.src_head;
	if (ex_insert_splitter_variable(state, &csr))
	{
		ex_destroy_token(csr.p.src_head);
		ex_destroy_token(csr.p.head);
		return (NULL);
	}
	return (csr.p.head);
}
