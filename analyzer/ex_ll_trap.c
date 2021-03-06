/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_ll_trap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/03 22:14:46 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/29 23:59:17 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

static void	ex_ll_push_back(t_ex_state *state, t_ex_unit_cursor *csr)
{
	if (csr->str[csr->i] && ft_strchr(EX_SPECIAL_VAR_CHAR, csr->str[csr->i]))
	{
		csr->substr_s = csr->vs + 1;
		csr->substr_e = csr->i + 1;
	}
	else
	{
		csr->running = XI_BARE;
		csr->substr_s = csr->vs;
		csr->substr_e = csr->i;
	}
	if (!ex_push_back_token(state, csr, NULL))
		ex_mark_failed(state, 1, "[LL] push back ex-token");
	csr->running = XI_NEUTRAL;
	csr->i = csr->substr_e;
}

// trap a char in a BARE word.
int	ex_ll_trap_bare(t_ex_state *state, t_ex_unit_cursor *csr)
{
	char	c;

	c = csr->str[csr->i];
	if ((c == '\'' && !csr->quote && !state->ignore_quote)
		|| (c == '"' && !state->ignore_quote)
		|| (!state->no_param && c == '$')
		|| !c)
	{
		csr->substr_s = csr->vs;
		csr->substr_e = csr->i;
		if (!ex_push_back_token(state, csr, NULL))
			ex_mark_failed(state, 1, "[LL-bare] push back ex-token");
		csr->running = XI_NEUTRAL;
		if (csr->quote == '"' && !c)
			return (0);
	}
	else
		csr->i += 1;
	return (1);
}

// trap a chsr in BRAVED_VAR.
// BRAVED_VAR will close by encounting '}'.
int	ex_ll_trap_braced_var(t_ex_state *state, t_ex_unit_cursor *csr)
{
	char	c;

	c = csr->str[csr->i];
	if (c == '}' || !c)
	{
		csr->substr_s = csr->vs + 2;
		csr->substr_e = csr->i;
		if (ex_ll_replace_var(state, csr))
			ex_mark_failed(state, 1, "[LL-bvar] push back ex-token");
		if (c)
			csr->i += 1;
		csr->running = XI_NEUTRAL;
	}
	else
		csr->i += 1;
	return (1);
}

// trap a char in VAR.
// VAR will transit to BRACED_VAR by encounting '{'.
int	ex_ll_trap_var(t_ex_state *state, t_ex_unit_cursor *csr)
{
	const char	c = csr->str[csr->i];

	if ((csr->i == csr->vs + 1 && (ft_isalpha(c) || c == '_'))
		|| (csr->i > csr->vs + 1 && (ft_isalnum(c) || c == '_')))
		csr->i += 1;
	else if (c == '{' && csr->i == csr->vs + 1)
	{
		csr->i += 1;
		csr->running = XI_BRACED_VAR;
	}
	else if (csr->i != csr->vs + 1 || ex_ll_now_at_special_var(csr, csr->i))
	{
		csr->substr_s = csr->vs + 1;
		csr->i += !!ex_ll_now_at_special_var(csr, csr->i);
		csr->substr_e = csr->i;
		if (ex_ll_replace_var(state, csr))
			ex_mark_failed(state, 1, "[LL-var] push back ex-token");
		csr->running = XI_NEUTRAL;
	}
	else
	{
		ex_ll_push_back(state, csr);
		csr->running = XI_NEUTRAL;
	}
	return (1);
}

// ASSUMED: state->running == XI_SQUOTED
// trap a char in SQUOTED.
// SQUOTED will close by '.
int	ex_ll_trap_squoted(t_ex_state *state, t_ex_unit_cursor *csr)
{
	char	c;

	c = csr->str[csr->i];
	if (c == '\'' || !c)
	{
		csr->substr_s = csr->vs + 1;
		csr->substr_e = csr->i;
		if (!ex_push_back_token(state, csr, NULL))
			ex_mark_failed(state, 1, "[LL-sq] push back ex-token");
		if (c)
			csr->i += 1;
		csr->running = XI_NEUTRAL;
	}
	else
		csr->i += 1;
	return (1);
}
