/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_ll_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 19:00:34 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/05 21:13:53 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

static int	ex_ll_err_bad_substitution(t_ex_state *state, t_ex_unit_cursor *csr)
{
	if (csr->running != XI_BRACED_VAR)
		ft_putstr_fd("[!!] something wrong: unexpected running type\n",
			STDERR_FILENO);
	ft_putstr_fd("-" MS_AZ_PROGNAME ": ", STDERR_FILENO);
	write(STDERR_FILENO, csr->str + csr->vs, csr->i + 1 - csr->vs);
	ft_putstr_fd(": bad substitution\n", STDERR_FILENO);
	state->error_printed = 1;
	return (ex_mark_failed(state, 1));
}

static int	ex_ll_validate_var_key(t_ex_state *state, t_ex_unit_cursor *csr)
{
	size_t	i;
	size_t	n;
	char	c;

	n = csr->substr_e - csr->substr_s;
	i = csr->substr_s;
	if (n == 0)
		return (ex_ll_err_bad_substitution(state, csr));
	if (n == 1)
	{
		if (!ft_strchr(EX_SPECIAL_VAR_CHAR, csr->str[i]))
			return (ex_ll_err_bad_substitution(state, csr));
		return (MS_AZ_SUCC);
	}
	while (i < csr->substr_e)
	{
		c = csr->str[i];
		if ((i == 0 && !ft_isalpha(c) && c != '_')
			|| (i > 0 && !ft_isalnum(c) && c != '_'))
			return (ex_ll_err_bad_substitution(state, csr));
		i += 1;
	}
	return (MS_AZ_SUCC);
}

static char	*ex_ll_replace_special_var(t_ex_state *state, t_ex_unit_cursor *csr)
{
	char	c;

	c = csr->str[csr->substr_s];
	if (c == '?')
		return (ft_itoa(state->last_exit_status));
	return (NULL);
}

static char	*ex_ll_replace_nominal_var(t_ex_state *state, t_ex_unit_cursor *csr)
{
	char		*key;
	char		*val;
	t_shellvar	*var;

	key = ft_substr(csr->str, csr->substr_s,
			csr->substr_e - csr->substr_s);
	if (!key)
	{
		state->failed = 1;
		return (NULL);
	}
	var = state->var;
	val = NULL;
	while (var)
	{
		if (!ft_strcmp(var->key, key))
		{
			if (var->value)
				val = ft_strdup(var->value);
			break ;
		}
		var = var->next;
	}
	free(key);
	return (val);
}

int	ex_ll_replace_var(t_ex_state *state, t_ex_unit_cursor *csr)
{
	char	*val;
	size_t	n;

	if (ex_ll_validate_var_key(state, csr))
		return (MS_AZ_FAIL);
	n = csr->substr_e - csr->substr_s;
	if (n == 1 && ft_strchr(EX_SPECIAL_VAR_CHAR, csr->str[csr->substr_s]))
		val = ex_ll_replace_special_var(state, csr);
	else
		val = ex_ll_replace_nominal_var(state, csr);
	if (!val)
		val = ft_strdup("");
	if (!val)
		return (MS_AZ_FAIL);
	csr->running = XI_VAR;
	if (ex_push_back_token(state, csr, val))
		return (MS_AZ_SUCC);
	free(val);
	return (MS_AZ_FAIL);
}
