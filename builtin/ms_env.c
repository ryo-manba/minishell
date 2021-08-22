#include "ms_builtin.h"

// environから一文字ずつ見ていって, "key = value"に分けて構造体を作る
t_shellvar *ms_create_env(void)
{
	extern char **environ;
	t_shellvar *env;
	int	i;
	int	j;
	char *key;
	char *value;

	i = 0;
	while (environ[i])
	{
		j = ft_strchr_i(environ[i], '='); // '='までの距離を図る
		key = ft_substr(environ[i], 0, j);
		value = ft_substr(environ[i], j + 1, strlen(environ[i]));
		ms_env_add_back(&env, ms_new_env(key, value, 1));
		free(key);
		free(value);
		i++;
	}
	return (env);
}

t_shellvar	*ms_new_env(char *key, char *value, int is_env)
{
	t_shellvar *new_env;

	new_env = malloc(sizeof(t_shellvar));
	if (new_env == NULL)
		perror("malloc");
	new_env->key = ft_strdup(key);
	new_env->value = ft_strdup(value);
	new_env->is_env = is_env;
	new_env->attr = 0;	// とりあえず保留
	new_env->next = NULL;
	return (new_env);
}

t_shellvar	*ms_envlast(t_shellvar *env)
{
	t_shellvar	*last;

	if (env == NULL)
		perror("malloc");
	last = env;
	while (last->next != NULL)
		last = last->next;
	return (last);
}

void	ms_env_add_back(t_shellvar **env, t_shellvar *new)
{
	t_shellvar *last;

	if (*env == NULL)
		*env = new;
	else
	{
		last = ms_envlast(*env);
		last->next = new;
	}
}

// envだけを入力された場合 環境変数をすべて出力する。
int	ms_env(t_shellvar *env)
{
	t_shellvar	*tmp;

	tmp = env;
	while (tmp)
	{
		printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}
