#include "ms_builtin.h"

#define KEY 0
#define VALUE 1
// environから一文字ずつ見ていって, "key = value"に分けて構造体を作る
t_shellvar *ms_create_env(void)
{
	extern char **environ;
	t_shellvar *env;
	int	i;
	int	j;
	char *key_value[2];

	env = NULL;
	i = 0;
	while (environ[i])
	{
		j = ft_strchr_i(environ[i], '='); // '='までの距離を図る
		key_value[KEY] = ft_substr(environ[i], 0, j);
		key_value[VALUE] = ft_substr(environ[i], j + 1, ft_strlen(environ[i]));
		blt_env_add_back(&env, blt_new_env(key_value[KEY], key_value[VALUE], 1));
		free(key_value[KEY]);
		free(key_value[VALUE]);
		i++;
	}
	return (env);
}

t_shellvar	*blt_new_env(char *key, char *value, int is_env)
{
	t_shellvar *new_env;

	new_env = malloc(sizeof(t_shellvar));
	if (new_env == NULL)
		perror("malloc");
	new_env->key = ft_strdup(key);
	if (value == NULL) // export VAR  =がつながってない場合
		new_env->value = NULL;
	else
		new_env->value = ft_strdup(value);
	new_env->is_env = is_env;
	new_env->attr = 0;	// とりあえず保留
	new_env->next = NULL;
	return (new_env);
}

t_shellvar	*blt_envlast(t_shellvar *env)
{
	t_shellvar	*last;

	if (env == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	last = env;
	while (last->next != NULL)
		last = last->next;
	return (last);
}

void	blt_env_add_back(t_shellvar **env, t_shellvar *new)
{
	t_shellvar *last;

	if (*env == NULL)
		*env = new;
	else
	{
		last = blt_envlast(*env);
		last->next = new;
	}
}

int	blt_env(t_shellvar *env)
{
	t_shellvar	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->value != NULL) // valueが設定されていなかったら出力しない
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}
