/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 18:00:09 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/23 19:56:45 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

int	blt_append_env(t_shellvar *env, char *key, char *value)
{
	t_shellvar	*append;

	append = ms_new_env(key, value);
	if (append == NULL)
	{
		ms_perror("malloc");
		return (MS_BLT_FAIL);
	}
	ms_env_add_back(&env, append);
	return (MS_BLT_SUCC);
}

int	blt_append_or_update_env(t_shellvar *env, char *key, char *value)
{
	t_shellvar	*key_pos;

	key_pos = ms_search_key(env, key);
	if (key_pos == NULL)
	{
		if (blt_append_env(env, key, value) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	else if (value != NULL)
	{
		if (blt_update_env(key_pos, value) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

int	blt_update_env(t_shellvar *update_pos, char *value)
{
	free(update_pos->value);
	update_pos->value = ft_strdup(value);
	if (update_pos->value == NULL)
	{
		ms_perror("malloc");
		return (MS_BLT_FAIL);
	}
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
