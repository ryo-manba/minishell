/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 17:59:54 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/08 17:30:26 by rmatsuka         ###   ########.fr       */
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

void	ms_env_all_free(t_shellvar *var)
{
	t_shellvar	*tmp;

	while (var)
	{
		tmp = var;
		var = var->next;
		ms_env_free(tmp);
	}
}

t_shellvar	*ms_envlast(t_shellvar *var)
{
	t_shellvar	*last;

	if (var == NULL)
		return (NULL);
	last = var;
	while (last->next != NULL)
		last = last->next;
	return (last);
}

void	ms_env_free(t_shellvar *env)
{
	free(env->key);
	free(env->value);
	free(env);
}
