/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_ll.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 19:00:19 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/29 23:29:56 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			ex_mark_failed(state, 1, "[LL] push back ex-token");
			return (NULL);
		}
	}
	return (ext);
}

// origin.
// transit from neutral to any other modes (except BRACED_BAR).
// and trap double-quote.
int	ex_ll_trap_neutral(t_ex_state *state, t_ex_unit_cursor *csr,
	int for_heredoc)
{
	if (csr->str[csr->i] == '\'' && !csr->quote && !state->ignore_quote)
	{
		csr->vs = csr->i++;
		csr->running = XI_SQUOTED;
	}
	else if (csr->str[csr->i] == '"' && !state->ignore_quote)
	{
		if (ex_ll_trap_dquote(state, csr, for_heredoc))
			ex_mark_failed(state, 1, "[LL-nt] push back ex-token");
	}
	else if (csr->str[csr->i] == '$' && !state->no_param)
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

void	ex_ll_unit(t_ex_state *state, t_ex_unit_cursor *csr, int for_heredoc)
{
	while (!state->failed)
	{
		if (csr->running == XI_NEUTRAL
			&& csr->str[csr->i] == csr->quote)
			break ;
		if (csr->running == XI_NEUTRAL
			&& ex_ll_trap_neutral(state, csr, for_heredoc))
			continue ;
		if (csr->running == XI_SQUOTED && ex_ll_trap_squoted(state, csr))
			continue ;
		if (csr->running == XI_VAR && !for_heredoc
			&& ex_ll_trap_var(state, csr))
			continue ;
		if (csr->running == XI_BRACED_VAR && !for_heredoc
			&& ex_ll_trap_braced_var(state, csr))
			continue ;
		if (csr->running == XI_BARE && ex_ll_trap_bare(state, csr))
			continue ;
		if (csr->running == XI_NEUTRAL
			&& !csr->str[csr->i])
			break ;
	}
}

t_ex_token	*ex_shell_param(t_ex_state *state, t_stree *stree, int for_heredoc)
{
	t_ex_unit_cursor	csr;

	ex_ll_init_cursor(&csr, stree->token_id, stree->token, '\0');
	ex_ll_unit(state, &csr, for_heredoc);
	ex_stringify_extoken_ifneeded(csr.p.head, "[LL]");
	return (csr.p.head);
}
