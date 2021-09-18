/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 17:59:54 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/18 10:00:33 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

void	ms_env_add_back(t_shellvar **var, t_shellvar *new_var)
{
	t_shellvar	*last;

	if (*var == NULL)
		*var = new_var;
	else
	{
		last = ms_envlast(*var);
		last->next = new_var;
	}
}

void	ms_env_all_free(t_shellvar **var)
{
	t_shellvar	*tmp;

	while (*var)
	{
		tmp = *var;
		*var = (*var)->next;
		ms_env_free(tmp);
	}
}

t_shellvar	*ms_envlast(t_shellvar *var)
{
	t_shellvar	*last;

	if (!var)
		return (NULL);
	last = var;
	while (last && last->next)
		last = last->next;
	return (last);
}

void	ms_env_free(t_shellvar *env)
{
	free(env->key);
	free(env->value);
	free(env);
}

int	ms_preset_env(t_shellvar **var)
{
	t_shellvar	*temp;
	char		*v;

	if (!ms_search_key(*var, "OLDPWD")
		&& blt_append_env(*var, "OLDPWD", NULL))
		return (1);
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
