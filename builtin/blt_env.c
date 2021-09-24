/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 17:59:57 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/24 17:48:43 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

int	blt_env(t_shellvar *var)
{
	t_shellvar	*tmp;

	tmp = var;
	while (tmp)
	{
		if (tmp->value != NULL)
		{
			ft_putstr_fd(tmp->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putendl_fd(tmp->value, STDOUT_FILENO);
		}
		tmp = tmp->next;
	}
	return (MS_BLT_SUCC);
}

// Update the env if it exists.
int	blt_create_or_update_env(t_shellvar *env, char *key, char *new_value)
{
	t_shellvar	*key_pos;

	key_pos = ms_search_key(env, key);
	if (key_pos)
	{
		if (blt_update_env(key_pos, new_value) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	else
	{
		key_pos = ms_new_env(key, new_value);
		if (key_pos == NULL)
			return (MS_BLT_FAIL);
		ms_env_add_back(&env, key_pos);
	}
	return (MS_BLT_SUCC);
}
