#include "ms_builtin.h"

// unsetで指定された要素が先頭だった場合
void	ms_unset_head(t_shellvar *env)
{
	t_shellvar	*tmp;

	tmp = env;
	env = env->next;
	ms_env_free(tmp);
}

// "unset b"  a->b->c を a->c free(b)
void	ms_unset_second_and_subsequent(t_shellvar *env, t_shellvar *key_pos)
{
	t_shellvar	*head;
	t_shellvar	*tmp;

	head = env;
	while (head)
	{
		if (head->next == key_pos) // 削除するkeyが見つかったらnextをつなぎ替える
		{
			tmp = head->next;
			head->next = head->next->next;
			ms_env_free(tmp);
			return ;
		}
		head = head->next;
	}
}

// keyを探して見つかったらリストから外す
int	ms_unset(t_shellvar *env, char *key)
{
	t_shellvar	*key_pos;
	t_shellvar	*head;
	t_shellvar	*tmp;

	key_pos	= ms_search_key(env, key);
	if (key_pos != NULL) // 存在しなかったら何もしない
	{

		if (env == key_pos) //先頭だった場合
		{
			ms_unset_head(env);
		}
		else // 2番目以降
		{
			ms_unset_second_and_subsequent(env, key_pos);
		}
	}
	return (0);
}
