#include "ms_builtin.h"

// sort済みの環境変数を出力する
int	ms_print_export(t_shellvar *env)
{
	t_shellvar *tmp;

	tmp = env;
	while (tmp)
	{
		printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

// 環境変数をクリアする
void	ms_env_clear(t_shellvar *env)
{
	t_shellvar *tmp;
	while (env)
	{
		tmp = env;
		env = env->next;
	free(tmp->value);
	free(tmp->key);
	free(tmp);
	}
}

// ソートするため環境変数のコピーを作る
t_shellvar *ms_copy_env(t_shellvar *env)
{
	t_shellvar *tmp;
	t_shellvar *copy;

	tmp = env;
	copy = NULL;
	while (tmp)
	{
		ms_env_add_back(&copy, ms_new_env(tmp->key, tmp->value, 1));
		tmp = tmp->next;
	}
	return (copy);
}

int	ms_export(t_shellvar *env, char *s)
{
	t_shellvar *copy;
	// ここの条件文変える
	if (ft_strcmp("export", s) == 0) // export 単体の場合は環境変数をソートして'declare -x hoge="huga"'の形式で出力する
	{
		copy = ms_copy_env(env); // 本体をsortしないようにコピーを作る。
		ms_quick_sort(copy, ms_envlast(copy));
		ms_print_export(copy);
		ms_env_clear(copy); // copyをfreeする
	}
	else // exportの処理
	{
		return (0);
	}
	return 0;
}