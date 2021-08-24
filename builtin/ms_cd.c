#include "ms_builtin.h"

void	ms_search_and_update_env(t_shellvar *env, char *key, char *new_value)
{
	t_shellvar	*key_pos;

	key_pos = ms_search_key(env, key);
	if (key_pos != NULL)
	{
		ms_update_env(key_pos, new_value);
	}
}

// PWD, OLDPWDが unsetされている場合は新しく作らない(minishell的には作っても良さそう)
int	ms_update_pwd(t_shellvar *env, char *old_pwd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (errno != 0)
	{
		perror("getcwd");
		return (1);
	}
	ms_search_and_update_env(env, "OLDPWD", old_pwd);
	ms_search_and_update_env(env, "PWD", pwd);
	return (0);
}

int	ms_cd_home(t_shellvar *env)
{
	t_shellvar *home;

	home = ms_search_key(env, "HOME");
	if (home == NULL)
	{
		printf("minishell: cd: HOME not set\n");
		return (-1);
	}
	else if (chdir(home->value) == -1)
	{
		perror(home->value);
		return (-1);
	}
	return (0);
}

// PWD がunsetされてたら参照できないので、移動前にgetcwdで保持しておく
int	ms_cd(t_shellvar *env, char *arg)
{
	char	*old_pwd;
	bool	is_success;

	is_success = true;
	old_pwd = getcwd(NULL, 0);
	if (errno != 0)
	{
		perror("getcwd");
		return (1);
	}
	if (arg == NULL) // home
	{
		if (ms_cd_home(env) == -1)
			is_success = false;
	}
	else
	{
		if (chdir(arg) == -1)
		{
			perror(arg);
			is_success = false;
		}
	}
	if (is_success == true)
	{
		if (ms_update_pwd(env, old_pwd) == 1)
			return (1);
	}
	free(old_pwd);
	return (0);
}
