/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_preset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 11:59:11 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/20 15:55:16 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

static int	ms_atoi_shlvl(char *str)
{
	char	*trimmed;
	size_t	n;
	int		rv;

	trimmed = ft_strtrim(str, " \t");
	if (!trimmed)
		return (0);
	n = 0;
	while (trimmed[n] && !!ft_strchr(" \t\n\r\v\f", trimmed[n]))
		n += 1;
	if (trimmed[n] && !!ft_strchr("+-", trimmed[n]))
		n += 1;
	while (trimmed[n])
	{
		if (!ft_isdigit(trimmed[n]))
			return (0);
		n += 1;
	}
	rv = ft_atoi(trimmed);
	free(trimmed);
	return (rv);
}

static int	ms_preset_env_oldpwd(t_shellvar **var)
{
	t_shellvar	*target;

	target = ms_search_key(*var, "OLDPWD");
	if (target)
		blt_delvalue_env(target);
	else if (blt_append_env(*var, "OLDPWD", NULL))
		return (1);
	return (0);
}

static int	ms_preset_env_shlvl(t_master *master)
{
	t_shellvar	*temp;
	int			lvl;
	char		*v;

	temp = ms_search_key(master->var, "SHLVL");
	if (!temp || !temp->value)
		return (!!blt_append_env(master->var, "SHLVL", "1"));
	lvl = ms_atoi_shlvl(temp->value) + 1;
	if (lvl >= 1000)
	{
		exec_print_prologue(master);
		ft_putstr_fd("shell level (", STDERR_FILENO);
		ft_putnbr_fd(lvl, STDERR_FILENO);
		ft_putendl_fd(") too high, resetting to 1", STDERR_FILENO);
		lvl = 1;
	}
	v = ft_itoa(lvl);
	if (!v || !!blt_update_env(temp, v))
	{
		free(v);
		return (1);
	}
	free(v);
	return (0);
}

int	ms_preset_env(t_master *master)
{
	if (ms_preset_env_oldpwd(&(master->var)))
		return (1);
	if (ms_preset_env_shlvl(master))
		return (1);
	return (0);
}
