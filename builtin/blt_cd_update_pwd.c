/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd_update_pwd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 15:29:59 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/23 20:36:17 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

int	blt_cd_update(t_master *master, t_shellvar *env, char *pwd)
{
	int			flag;
	char		*old_pwd;
	t_shellvar	*env_pwd;

	flag = 0;
	free(master->pwd);
	master->pwd = ft_strdup(pwd);
	env_pwd = ms_search_key(env, "PWD");
	if (env_pwd && env_pwd->value)
		old_pwd = ft_strdup(env_pwd->value);
	else
		old_pwd = ft_strdup("");
	if (old_pwd == NULL || master->pwd == NULL)
		return (1);
	if (!((blt_search_and_update_env(env, "OLDPWD", old_pwd) == MS_BLT_SUCC) && \
		blt_search_and_update_env(env, "PWD", master->pwd) == MS_BLT_SUCC))
		flag = 1;
	free(old_pwd);
	return (flag);
}

// If PWD and OLDPWD are unset, do not create a new one.
int	blt_cd_update_pwd(
	t_master *master, t_stree *tree, t_shellvar *env, int is_succ)
{
	char	*pwd;
	int		flag;

	errno = 0;
	pwd = getcwd(NULL, 0);
	if (errno != 0)
	{
		if (tree == NULL)
			return (MS_BLT_FAIL);
		return (blt_cd_no_current(master, env, tree->token));
	}
	else if (tree)
	{
		if (is_succ)
		{
			free(pwd);
			pwd = blt_cd_has_args(master, tree);
			if (pwd == NULL)
				return (MS_BLT_FAIL);
		}
	}
	flag = blt_cd_update(master, env, pwd);
	free(pwd);
	return (flag);
}
