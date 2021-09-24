/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 18:00:09 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/24 21:57:16 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

int	blt_append_env(t_shellvar *env, char *key, char *value, int is_env)
{
	t_shellvar	*append;

	append = ms_new_env(key, value, is_env);
	if (append == NULL)
	{
		ms_perror("malloc");
		return (MS_BLT_FAIL);
	}
	ms_env_add_back(&env, append);
	return (MS_BLT_SUCC);
}

int	blt_append_or_update_env(t_shellvar *env, char *key, char *value,
	int is_env)
{
	t_shellvar	*key_pos;

	key_pos = ms_search_key(env, key);
	if (key_pos == NULL)
	{
		if (blt_append_env(env, key, value, is_env) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	else
	{
		if (blt_update_env(key_pos, value, is_env) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

int	blt_update_env(t_shellvar *update_pos, char *value, int is_env)
{
	if (value)
	{
		free(update_pos->value);
		update_pos->value = ft_strdup(value);
		if (update_pos->value == NULL)
		{
			ms_perror("malloc");
			return (MS_BLT_FAIL);
		}
	}
	if (is_env)
		update_pos->is_env = 1;
	return (MS_BLT_SUCC);
}

// delete value of given var(will not unset)
int	blt_delvalue_env(t_shellvar *update_pos)
{
	free(update_pos->value);
	update_pos->value = NULL;
	return (MS_BLT_SUCC);
}

t_shellvar	*ms_search_key(t_shellvar *env, char *key)
{
	t_shellvar	*key_pos;

	key_pos = env;
	while (key_pos)
	{
		if (ft_strcmp(key_pos->key, key) == 0)
			break ;
		key_pos = key_pos->next;
	}
	return (key_pos);
}
