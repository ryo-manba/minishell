/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 17:59:57 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/06 17:59:58 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

// keyがある場合更新する
int	blt_search_and_update_env(t_shellvar *env, char *key, char *new_value)
{
	t_shellvar	*key_pos;

	key_pos = blt_search_key(env, key);
	if (key_pos != NULL)
	{
		if (blt_update_env(key_pos, new_value) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	return (MS_BLT_FAIL);
}

// ここはエクスパンダー絡むはず
int	ms_create_key_value(char *env, char *key_value[2])
{
	int	equal_idx;

	equal_idx = ft_strchr_i(env, '='); // '='までの距離を図る
	key_value[KEY] = ft_substr(env, 0, equal_idx);
	if (key_value[KEY] == NULL)
		return (MS_BLT_FAIL);
	key_value[VALUE] = ft_substr(env, equal_idx + 1, ft_strlen(env));
	if (key_value[VALUE] == NULL)
	{
		free(key_value[KEY]);
		return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

// environから一文字ずつ見ていって, "key = value"に分けて構造体を作る
t_shellvar *ms_create_env(void)
{
	extern char	**environ;
	char 		*key_value[2];
	t_shellvar	*var;
	int			i;

	i = 0;
	var = NULL;
	while (environ[i])
	{
		if (ms_create_key_value(environ[i], key_value) == MS_BLT_FAIL)
		{
			ms_env_all_free(var);
			return (NULL);
		}
		blt_env_add_back(&var, blt_new_env(key_value[KEY], key_value[VALUE], 1));
		free(key_value[KEY]);
		free(key_value[VALUE]);
		i++;
	}
	return (var);
}

int	blt_env(t_shellvar *var)
{
	t_shellvar	*tmp;

	tmp = var;
	while (tmp)
	{
		if (tmp->value != NULL) // valueが設定されていなかったら出力しない
		{
			ft_putstr_fd(tmp->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putendl_fd(tmp->value, STDOUT_FILENO);
		}
		tmp = tmp->next;
	}
	return (MS_BLT_SUCC);
}
