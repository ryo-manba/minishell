#include "ms_builtin.h"

t_shellvar	*ms_search_key(t_shellvar *env, char *key)
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
void	ms_append_env(t_shellvar *env, char *key, char *value)
{
	t_shellvar	*append;

	append = ms_new_env(key, value, 1);
	ms_env_add_back(&env, append);
}

// 環境変数のvalueを更新する
void	ms_update_env(t_shellvar *update_pos, char *value)
{
	free(update_pos->value);
	update_pos->value = ft_strdup(value);
}

// もしkeyが含まれていたら更新する、なかったら新しく作る
// "export VAR", "export VAR=" の場合も考慮する
int	ms_append_or_update_env(t_shellvar *env, char *key, char *value)
{
	t_shellvar	*key_pos;

	key_pos	= ms_search_key(env, key);
	if (key_pos == NULL) // keyがなかった場合
	{
		ms_append_env(env, key, value);
	}
	else if (value != NULL)// keyだけの場合は更新しない
	{
		ms_update_env(key_pos, value);
	}
	return (0);
}
