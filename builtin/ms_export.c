#include "ms_builtin.h"

// sort済みの環境変数を出力する
int	ms_print_export(t_shellvar *env)
{
	t_shellvar *tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->value == NULL)
			printf("declare -x %s\n", tmp->key);
		else
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

// 環境変数をクリアする
void	ms_env_clear(t_shellvar *env)
{
	t_shellvar	*tmp;

	tmp = env;
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

void	ms_print_sort_env(t_shellvar *env)
{
	t_shellvar *copy;

	copy = ms_copy_env(env); // 本体をsortしないようにコピーを作る。
	ms_quick_sort(copy, ms_envlast(copy));
	ms_print_export(copy);
	ms_env_clear(copy); // copyをfreeする
}

static void ms_export_test(t_shellvar *env)
{
	ms_append_or_update_env(env, "key", "update");
	ms_append_or_update_env(env, "empty_string", "");
	ms_append_or_update_env(env, "null", NULL);
	ms_append_or_update_env(env, "null_update", "null");
	ms_append_or_update_env(env, "null_update", NULL);
	ms_append_or_update_env(env, "empty_update", "empty");
	ms_append_or_update_env(env, "empty_update", "");
	ms_print_export(env);
}

// exportの引数どうするか
int	ms_export(t_shellvar *env, char *s)
{
	// ここの条件文変える
	if (ft_strcmp("export", s) == 1) // export 単体の場合は環境変数をソートして'declare -x hoge="huga"'の形式で出力する
	{
		ms_print_sort_env(env);
	}
	else // 環境変数を追加または更新する
	{
		ms_append_or_update_env(env, "key", "value");
		ms_export_test(env);
	}
	return (0);
}
