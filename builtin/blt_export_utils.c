/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 18:00:09 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/19 20:47:56 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

// 環境変数を追加する
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

// もしkeyが含まれていたら更新する、なかったら新しく作る
// "export VAR", "export VAR=" の場合も考慮する
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

// 環境変数のvalueを更新する
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
