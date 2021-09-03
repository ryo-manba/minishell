#include "ms_builtin.h"

// unsetで指定された要素が先頭だった場合
void	blt_unset_head(t_shellvar *env)
{
	t_shellvar	*tmp;

	tmp = env;
	env = env->next;
	blt_env_free(tmp);
}

// "unset b"  a->b->c を a->c free(b)
void	blt_unset_second_and_subsequent(t_shellvar *env, t_shellvar *key_pos)
{
	t_shellvar	*head;
	t_shellvar	*tmp;

	head = env;
	while (head->next != key_pos) // 削除するkeyが見つかるまで進める
	{
		head = head->next;
	}
	tmp = head->next;
	head->next = head->next->next;
	blt_env_free(tmp);
}

/**
 * $ unset PWD USER
 * tree->token = PWD
 * tree->right->token = USER
 */
// keyを探して見つかったらリストから外す
int	blt_unset(t_shellvar *env, t_stree *tree)
{
	t_shellvar	*key_pos;

	while (tree != NULL) // unset 単体の場合は何もしない
	{
		key_pos	= blt_search_key(env, tree->token);
		if (key_pos != NULL) // 存在しなかったら何もしない
		{
			if (env == key_pos) //先頭だった場合
			{
				blt_unset_head(env);
			}
			else // 2番目以降
			{
				blt_unset_second_and_subsequent(env, key_pos);
			}
		}
		tree = tree->right;
	}
	return (0);
}

