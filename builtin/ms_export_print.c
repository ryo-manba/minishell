#include "ms_builtin.h"

// sort済みの環境変数を出力する
void	blt_print_export(t_shellvar *env)
{
	t_shellvar *tmp;

	tmp = env;
	while (tmp)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		if (tmp->value == NULL)
		{
			ft_putendl_fd(tmp->key, STDOUT_FILENO);
		}
		else
		{
			ft_putstr_fd(tmp->key, STDOUT_FILENO);
			ft_putstr_fd('=\"', STDOUT_FILENO);
			if (ft_strncmp(tmp->value, "\\", 1) == 0) // export a="\n"　declare -x a="\\n"にする
				ft_putchar_fd('\\', STDOUT_FILENO);
			ft_putstr_fd(tmp->value, STDOUT_FILENO);
			ft_putendl_fd("\"", STDOUT_FILENO);
		}
		tmp = tmp->next;
	}
}

// 環境変数をクリアする
void	blt_env_clear(t_shellvar *env)
{
	t_shellvar	*tmp;

	tmp = env;
	while (env)
	{
		tmp = env;
		env = env->next;
		blt_env_free(tmp);
	}
}

// ソートするため環境変数のコピーを作る
t_shellvar *blt_copy_env(t_shellvar *env)
{
	t_shellvar *tmp;
	t_shellvar *copy;

	tmp = env;
	copy = NULL;
	while (tmp)
	{
		blt_env_add_back(&copy, blt_new_env(tmp->key, tmp->value, 1));
		tmp = tmp->next;
	}
	return (copy);
}

void	blt_print_sort_env(t_shellvar *env)
{
	t_shellvar *copy;

	copy = blt_copy_env(env); // 本体をsortしないようにコピーを作る。
	blt_quick_sort(copy, blt_envlast(copy));
	blt_print_export(copy);
	blt_env_clear(copy); // copyをfreeする
}
