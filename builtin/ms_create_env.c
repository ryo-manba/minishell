/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_create_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 14:53:27 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/30 20:12:13 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

int	ms_check_malloc_key_value(t_shellvar *var, char *s, int key_or_value)
{
	if (key_or_value == KEY)
	{
		var->key = ft_strdup(s);
		if (var->key == NULL)
		{
			free(var);
			ms_perror("malloc");
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
			ms_perror("malloc");
			return (MS_BLT_FAIL);
		}
	}
	return (MS_BLT_SUCC);
}

t_shellvar	*ms_create_append_env(char *env)
{
	t_shellvar	*append_var;
	char		*key_value[2];

	if (ms_create_key_value(env, key_value) == MS_BLT_FAIL)
		return (NULL);
	append_var = ms_new_env(key_value[KEY], key_value[VALUE], 1);
	free(key_value[KEY]);
	free(key_value[VALUE]);
	if (append_var == NULL)
		return (NULL);
	return (append_var);
}

int	ms_create_env(t_master *master)
{
	extern char	**environ;
	t_shellvar	*append_var;
	size_t		i;

	i = 0;
	while (environ[i])
	{
		append_var = ms_create_append_env(environ[i]);
		if (append_var == NULL)
		{
			ms_env_all_free(&(master->var));
			return (1);
		}
		append_var->is_env = 1;
		ms_env_add_back(&(master->var), append_var);
		i++;
	}
	if (ms_preset_env(master))
	{
		ms_env_all_free(&(master->var));
		return (1);
	}
	return (0);
}

int	ms_create_key_value(char *env, char *key_value[2])
{
	ssize_t	equal_idx;

	equal_idx = ft_strchr_i(env, '=');
	key_value[KEY] = ft_substr(env, 0, equal_idx);
	if (key_value[KEY] == NULL)
	{
		ms_perror("malloc");
		return (MS_BLT_FAIL);
	}
	key_value[VALUE] = ft_substr(env, equal_idx + 1, ft_strlen(env));
	if (key_value[VALUE] == NULL)
	{
		ms_perror("malloc");
		free(key_value[KEY]);
		return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

t_shellvar	*ms_new_env(char *key, char *value, int is_env)
{
	t_shellvar	*new_env;

	new_env = (t_shellvar *)ft_calloc(1, sizeof(t_shellvar));
	if (new_env == NULL)
	{
		ms_perror("malloc");
		return (NULL);
	}
	if (ms_check_malloc_key_value(new_env, key, KEY) == MS_BLT_FAIL)
		return (NULL);
	if (is_env)
		new_env->is_env = 1;
	if (value == NULL)
		new_env->value = NULL;
	else
	{
		if (ms_check_malloc_key_value(new_env, value, VALUE) == MS_BLT_FAIL)
			return (NULL);
	}
	new_env->next = NULL;
	return (new_env);
}
