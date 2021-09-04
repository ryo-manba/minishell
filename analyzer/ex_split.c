#include "ms_analyzer.h"

int	ex_push_back_divider(t_ex_state *state, t_ex_unit_cursor *csr,
		t_ex_token *token)
{
	t_ex_token	*divider;

	if (!csr->p.tail)
		return (MS_AZ_SUCC);
	divider = (t_ex_token *)ft_calloc(1, sizeof(t_ex_token));
	if (!divider)
	{
		state->failed = 1;
		return (MS_AZ_FAIL);
	}
	divider->token_id = XI_DIVIDER;
	divider->pa_token_id = token->pa_token_id;
	csr->p.tail->right = divider;
	csr->p.tail = divider;
	return (MS_AZ_SUCC);
}

int	ex_clone_and_push_back_token(t_ex_state *state, t_ex_unit_cursor *csr,
		t_ex_token *token)
{
	t_ex_token	*cloned_ext;
	char		*cloned_str;

	cloned_ext = (t_ex_token *)malloc(sizeof(t_ex_token));
	cloned_str = ft_substr(token->token, csr->vs, csr->i - csr->vs);
	if (!cloned_ext || !cloned_str)
	{
		free(cloned_ext);
		free(cloned_str);
		state->failed = 1;
		return (MS_AZ_FAIL);
	}
	ft_memcpy(cloned_ext, token, sizeof(t_ex_token));
	cloned_ext->token = cloned_str;
	cloned_ext->right = NULL;
	if (csr->p.tail)
		csr->p.tail->right = cloned_ext;
	csr->p.tail = cloned_ext;
	if (!csr->p.head)
		csr->p.head = csr->p.tail;
	return (MS_AZ_SUCC);
}

t_ex_token	*ex_split(t_ex_state *state, t_ex_token *token)
{
	t_ex_unit_cursor	csr;
	t_ex_token			*temp;

	ex_init_cursor_mid(&csr, token);
	while (!state->failed)
	{
		temp = csr.s.tail;
		if (!temp)
			break ;
		if (temp->token_id != XI_VAR)
		{
			// exTokenがVARでない場合は、exトークンをコピーする。
			csr.vs = 0;
			csr.i = ft_strlen(temp->token);
			if (ex_clone_and_push_back_token(state, &csr, temp))
			{
				state->failed = 1;
				break ;
			}
		}
		else
		{
			// トークン文字列を先頭から見ていく。
			csr.vs = 0;
			csr.i = 0;
			while (!state->failed)
			{
				if (!temp->token[csr.i]) // 処理を終了する。
					break ;
				else if (ft_strchr(EX_IFS, temp->token[csr.i]))
				{
					// 空白文字でなくなるまでiを進める。
					while (temp->token[csr.i] && ft_strchr(EX_IFS, temp->token[csr.i]))
						csr.i += 1;
					// 一時変数t_tailがNULLでないなら、DIVIDERトークンを追加する。
					if (ex_push_back_divider(state, &csr, temp))
						break; 
				}
				else
				{
					// 現在位置の文字が空白文字またはNULになるまでiを進める。
					csr.vs = csr.i;
					while (temp->token[csr.i] && !ft_strchr(EX_IFS, temp->token[csr.i]))
						csr.i += 1;
					// vsからi-1までを新しいexTokenとして切り出す。
					if (ex_clone_and_push_back_token(state, &csr, temp))
						break ;
				}
			}
		}
		csr.s.tail = temp->right;
	}
	ex_destroy_token(csr.s.head);
	if (state->failed)
	{
		ex_destroy_token(csr.p.head);
		return (NULL);
	}
	return (csr.p.head);
}
