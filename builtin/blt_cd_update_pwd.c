/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd_update_pwd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 15:29:59 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/30 20:04:05 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

int	blt_cd_update(t_master *master, t_shellvar *env, char *pwd)
{
	int			ret;
	char		*old_pwd;
	t_shellvar	*env_pwd_pos;

	ret = 0;
	free(master->pwd);
	master->pwd = ft_strdup(pwd);
	env_pwd_pos = ms_search_key(env, "PWD");
	if (env_pwd_pos && env_pwd_pos->value)
		old_pwd = ft_strdup(env_pwd_pos->value);
	else
		old_pwd = ft_strdup("");
	if (old_pwd == NULL || master->pwd == NULL)
		return (1);
	if (blt_append_or_update_env(&env, "OLDPWD", old_pwd, 0) || \
		blt_append_or_update_env(&env, "PWD", master->pwd, 0))
		ret = 1;
	free(old_pwd);
	return (ret);
}

// If PWD and OLDPWD are unset, do not create a new one.
int	blt_cd_update_pwd(
	t_master *master, t_stree *tree, t_shellvar *env, int is_succ)
{
	char	*pwd;
	int		ret;

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
	ret = blt_cd_update(master, env, pwd);
	free(pwd);
	return (ret);
}
