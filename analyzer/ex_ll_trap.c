/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_ll_trap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/03 22:14:46 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/05 21:50:13 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

static void	ex_ll_push_back(t_ex_state *state, t_ex_unit_cursor *csr)
{
	if (ft_strchr(EX_SPECIAL_VAR_CHAR, csr->str[csr->i]))
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

int	ex_ll_trap_bare(t_ex_state *state, t_ex_unit_cursor *csr)
{
	char	c;

	c = csr->str[csr->i];
	if ((c == '\'' && !csr->quote) || c == '"' || c == '$' || !c)
	{
		csr->substr_s = csr->vs;
		csr->substr_e = csr->i;
		if (!ex_push_back_token(state, csr, NULL))
			ex_mark_failed(state, 1, "[LL-bare] push back ex-token");
		csr->running = XI_NEUTRAL;
	}
	else
		csr->i += 1;
	return (1);
}

// }で解除される。
int	ex_ll_trap_braced_var(t_ex_state *state, t_ex_unit_cursor *csr)
{
	char	c;

	(void)state;
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

// BRACED_VARに遷移することがある。
int	ex_ll_trap_var(t_ex_state *state, t_ex_unit_cursor *csr)
{
	char	c;

	c = csr->str[csr->i];
	if ((csr->i == csr->vs + 1 && (ft_isalpha(c) || c == '_'))
		|| (csr->i > csr->vs + 1 && (ft_isalnum(c) || c == '_')))
		csr->i += 1;
	else if (c == '{' && csr->i == csr->vs + 1)
	{
		csr->i += 1;
		csr->running = XI_BRACED_VAR;
	}
	else if (csr->i == csr->vs + 1)
		ex_ll_push_back(state, csr);
	else
	{
		csr->substr_s = csr->vs + 1;
		csr->substr_e = csr->i;
		if (ex_ll_replace_var(state, csr))
			ex_mark_failed(state, 1, "[LL-var] push back ex-token");
		csr->running = XI_NEUTRAL;
	}
	return (1);
}

// シングルクオートはシングルクオートで解除される。
int	ex_ll_trap_squoted(t_ex_state *state, t_ex_unit_cursor *csr)
{
	char	c;

	c = csr->str[csr->i];
	if (csr->running != XI_SQUOTED)
		return (0);
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
