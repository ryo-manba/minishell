/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_preset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 11:59:11 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/19 20:48:08 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

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

static int	ms_preset_env_shlvl(t_shellvar **var)
{
	t_shellvar	*temp;
	char		*v;

	temp = ms_search_key(*var, "SHLVL");
	if (!temp || !temp->value)
	{
		if (blt_append_env(*var, "SHLVL", "1"))
			return (1);
	}
	else
	{
		v = ft_itoa(ft_atoi(temp->value) + 1);
		if (!v || !!blt_update_env(temp, v))
		{
			free(v);
			return (1);
		}
		free(v);
	}
	return (0);
}

int	ms_preset_env(t_shellvar **var)
{
	if (ms_preset_env_oldpwd(var))
		return (1);
	if (ms_preset_env_shlvl(var))
		return (1);
	return (0);
}
