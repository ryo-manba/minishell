/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_create_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 14:53:27 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/10 03:30:00 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

// keyとvalueに分けて,失敗したらfreeする
int	ms_check_malloc_key_value(t_shellvar *var, char *s, int key_or_value)
{
	if (key_or_value == KEY)
	{
		var->key = ft_strdup(s);
		if (var->key == NULL)
		{
			free(var);
			ms_print_perror("malloc");
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
			ms_print_perror("malloc");
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
	append_var = ms_new_env(key_value[KEY], key_value[VALUE]);
	if (append_var == NULL)
	{
		free(key_value[KEY]);
		free(key_value[VALUE]);
		return (NULL);
	}
	return (append_var);
}

// environから一文字ずつ見ていって, "key = value"に分けて構造体を作る
t_shellvar	*ms_create_env(void)
{
	extern char	**environ;
	t_shellvar	*var;
	t_shellvar	*append_var;
	int			i;

	i = 0;
	var = NULL;
	while (environ[i])
	{
		append_var = ms_create_append_env(environ[i]);
		if (append_var == NULL)
		{
			ms_env_all_free(var);
			return (NULL);
		}
		ms_env_add_back(&var, append_var);
		i++;
	}
	return (var);
}

// =でkeyとvalueを分ける
int	ms_create_key_value(char *env, char *key_value[2])
{
	int	equal_idx;

	equal_idx = ft_strchr_i(env, '=');
	key_value[KEY] = ft_substr(env, 0, equal_idx);
	if (key_value[KEY] == NULL)
	{
		ms_print_perror("malloc");
		return (MS_BLT_FAIL);
	}
	key_value[VALUE] = ft_substr(env, equal_idx + 1, ft_strlen(env));
	if (key_value[VALUE] == NULL)
	{
		ms_print_perror("malloc");
		free(key_value[KEY]);
		return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

// =がない場合はvalueをNULLにする
t_shellvar	*ms_new_env(char *key, char *value)
{
	t_shellvar	*new_env;

	new_env = (t_shellvar *)ft_calloc(1, sizeof(t_shellvar));
	if (new_env == NULL)
	{
		ms_print_perror("malloc");
		return (NULL);
	}
	if (ms_check_malloc_key_value(new_env, key, KEY) == MS_BLT_FAIL)
		return (NULL);
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
