#include "ms_analyzer.h"

static int	ex_ll_err_bad_substitution(t_ex_state *state, t_ex_unit_cursor *csr)
{
	state->failed = MS_AZ_FAIL;
	if (csr->running != XI_BRACED_VAR)
		ft_putstr_fd("[!!] something wrong: unexpected running type\n",
			STDERR_FILENO);
	ft_putstr_fd("-" MS_AZ_PROGNAME ": ", STDERR_FILENO);
	write(STDERR_FILENO, csr->str + csr->vs, csr->i + 1 - csr->vs);
	ft_putstr_fd(": bad substitution\n", STDERR_FILENO);
	return (state->failed);
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
	if (n == 1 && !ft_strchr(EX_SPECIAL_VAR, csr->str[i]))
		return (ex_ll_err_bad_substitution(state, csr));
	if (n == 1)
		return (MS_AZ_SUCC);
	while (i < csr->substr_e)
	{
		c = csr->str[i];
		if (i == 0 && !ft_isalpha(c) && c != '_')
			return (ex_ll_err_bad_substitution(state, csr));
		if (i > 0 && !ft_isalnum(c) && c != '_')
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
	char		*val;
	size_t		n;

	if (ex_ll_validate_var_key(state, csr))
		return (state->failed);
	n = csr->substr_e - csr->substr_s;
	if (n == 1 && ft_strchr(EX_SPECIAL_VAR, csr->str[csr->substr_s]))
		val = ex_ll_replace_special_var(state, csr);
	else
		val = ex_ll_replace_nominal_var(state, csr);
	if (!val)
		val = ft_strdup("");
	if (val)
	{
		csr->running = XI_VAR;
		if (ex_push_back_token(state, csr, val))
			return (MS_AZ_SUCC);
		free(val);
	}
	return (MS_AZ_FAIL);
}
