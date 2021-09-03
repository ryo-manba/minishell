#include "ms_builtin.h"

void	blt_env_free(t_shellvar *env)
{
	free(env->key);
	free(env->value);
	free(env);
}

t_shellvar	*blt_search_key(t_shellvar *env, char *key)
{
	t_shellvar	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			break ;
		tmp = tmp->next;
	}
	return (tmp); // keyの場所か,NULLを返す
}

// 環境変数を追加する
int	blt_append_env(t_shellvar *env, char *key, char *value)
{
	t_shellvar	*append;

	append = blt_new_env(key, value, 1);
	if (append == NULL)
	{
		perrro("malloc");
		return (MS_BLT_FAIL);
	}
	blt_env_add_back(&env, append);
	return (MS_BLT_SUCC);
}

// 環境変数のvalueを更新する
int	blt_update_env(t_shellvar *update_pos, char *value)
{
	free(update_pos->value);
	update_pos->value = ft_strdup(value);
	if (update_pos->value == NULL)
	{
		perror("malloc");
		return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

// もしkeyが含まれていたら更新する、なかったら新しく作る
// "export VAR", "export VAR=" の場合も考慮する
int	blt_append_or_update_env(t_shellvar *env, char *key, char *value)
{
	t_shellvar	*key_pos;

	key_pos	= blt_search_key(env, key);
	if (key_pos == NULL) // keyがなかった場合
	{
		if (blt_append_env(env, key, value) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	else if (value != NULL)// keyだけの場合は更新しない
	{
		if (blt_update_env(key_pos, value) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}
