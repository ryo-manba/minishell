/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_env_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 17:59:54 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/06 17:59:55 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

// keyとvalueに分けて,失敗したらfreeする
int	blt_check_malloc_key_value(t_shellvar *var, char *s, int key_or_value)
{
	if (key_or_value == KEY)
	{
		var->key = ft_strdup(s);
		if (var->key == NULL)
		{
			free(var);
			return (MS_BLT_FAIL);
		}
	}
	if (key_or_value == VALUE)
	{
		var->value = ft_strdup(s);
		if (var->value == NULL)
		{
			free(var->key);
			free(var);
			return (MS_BLT_FAIL);
		}
	}
	return (MS_BLT_SUCC);
}

void	blt_env_add_back(t_shellvar **var, t_shellvar *new)
{
	t_shellvar	*last;

	if (*var == NULL)
		*var = new;
	else
	{
		last = blt_envlast(*var);
		last->next = new;
	}
}

t_shellvar	*blt_envlast(t_shellvar *var)
{
	t_shellvar	*last;

	if (var == NULL)
		return (NULL);
	last = var;
	while (last->next != NULL)
		last = last->next;
	return (last);
}

t_shellvar	*blt_new_env(char *key, char *value, int is_env)
{
	t_shellvar *new_env;

	new_env = (t_shellvar *)malloc(sizeof(t_shellvar));
	if (new_env == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	if (blt_check_malloc_key_value(new_env, key, KEY) == MS_BLT_FAIL)
		return (NULL);
	if (value == NULL) // export VAR  =がつながってない場合
		new_env->value = NULL;
	else
	{
		if (blt_check_malloc_key_value(new_env, value, VALUE) == MS_BLT_FAIL)
			return (NULL);
	}
	new_env->is_env = is_env;
	new_env->attr = 0;	// とりあえず保留
	new_env->next = NULL;
	return (new_env);
}

void	ms_env_all_free(t_shellvar *var)
{
	t_shellvar *tmp;

	while (var)
	{
		tmp = var;
		var = var->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}
