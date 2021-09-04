/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/04 13:23:15 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/04 15:19:24 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

int	ex_push_back_divider_if_needed(t_ex_state *state, t_ex_unit_cursor *csr,
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

t_ex_token	*ex_clone_and_push_back_token(t_ex_state *state,
		t_ex_unit_cursor *csr, t_ex_token *token)
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
		return (NULL);
	}
	ft_memcpy(cloned_ext, token, sizeof(t_ex_token));
	cloned_ext->token = cloned_str;
	cloned_ext->right = NULL;
	if (csr->p.tail)
		csr->p.tail->right = cloned_ext;
	csr->p.tail = cloned_ext;
	if (!csr->p.head)
		csr->p.head = csr->p.tail;
	return (cloned_ext);
}

int	ex_split_var_token(t_ex_state *state, t_ex_unit_cursor *csr,
	t_ex_token *temp)
{
	csr->vs = 0;
	csr->i = 0;
	while (!state->failed && temp->token[csr->i])
	{
		if (ft_strchr(EX_IFS, temp->token[csr->i]))
		{
			while (temp->token[csr->i]
				&& ft_strchr(EX_IFS, temp->token[csr->i]))
				csr->i += 1;
			ex_push_back_divider_if_needed(state, csr, temp);
		}
		else
		{
			csr->vs = csr->i;
			while (temp->token[csr->i]
				&& !ft_strchr(EX_IFS, temp->token[csr->i]))
				csr->i += 1;
			ex_clone_and_push_back_token(state, csr, temp);
		}
	}
	return (!!state->failed);
}

t_ex_token	*ex_split(t_ex_state *state, t_ex_token *token)
{
	t_ex_unit_cursor	csr;
	t_ex_token			*temp;

	ex_init_cursor_mid(&csr, token);
	while (csr.s.tail)
	{
		temp = csr.s.tail;
		if (temp->token_id != XI_VAR)
		{
			csr.vs = 0;
			csr.i = ft_strlen(temp->token);
			if (!ex_clone_and_push_back_token(state, &csr, temp))
				break ;
		}
		else if (ex_split_var_token(state, &csr, temp))
			break ;
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
