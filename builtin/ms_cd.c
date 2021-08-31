#include "ms_builtin.h"

// keyがある場合更新する
void	blt_search_and_update_env(t_shellvar *env, char *key, char *new_value)
{
	t_shellvar	*key_pos;

	key_pos = blt_search_key(env, key);
	if (key_pos != NULL)
	{
		blt_update_env(key_pos, new_value);
	}
}

// PWD, OLDPWDが unsetされている場合は新しく作らない
int	blt_update_pwd(t_shellvar *env, char *old_pwd)
{
	char	*pwd;

	errno = 0;
	pwd = getcwd(NULL, 0);
	if (errno != 0)
	{
		blt_cd_print_error(NULL , NULL);
		return (1);
	}
	blt_search_and_update_env(env, "OLDPWD", old_pwd);
	blt_search_and_update_env(env, "PWD", pwd);
	return (0);
}

int	blt_cd_home(t_shellvar *env)
{
	t_shellvar *home_pos;

	home_pos = blt_search_key(env, "HOME");
	if (home_pos == NULL)
	{
		blt_cd_print_error("HOME", "not set");
		return (1);
	}
	else if (chdir(home_pos->value) == -1)
	{
		blt_cd_print_error(home_pos->value, "No such file or directory");
		return (1);
	}
	return (0);
}

int	blt_change_directory(t_shellvar *env, t_stree *tree)
{
	if (tree == NULL) // home
	{
		if (blt_cd_home(env) == 1)
			return (1);
	}
	else
	{
		if (chdir(tree->token) == -1)
		{
			perror(tree->token);
			return (1);
		}
	}
}

// PWD がunsetされてたら参照できないので、移動前にgetcwdで保持しておく
// 第一引数のみ適用される、それ以降は無視される
int	blt_cd(t_shellvar *env, t_stree *tree)
{
	char	*old_pwd;

	if (tree != NULL && ft_strlen(tree->token) > 255)
	{
		blt_cd_print_error(tree->token, "File name too long");
		return (1);
	}
	errno = 0;
	old_pwd = getcwd(NULL, 0);
	if (errno != 0)
	{
		blt_cd_print_error(NULL, NULL);
		return (1);
	}
	if (blt_change_directory(env, tree) == 0)
	{
		if (blt_update_pwd(env, old_pwd) == 1)
			return (1);
	}
	free(old_pwd);
	return (0);
}
