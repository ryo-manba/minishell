/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd_update_pwd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 15:29:59 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/23 15:56:50 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

int	blt_cd_update(t_master *master, t_shellvar *env, char *pwd)
{
	int		flag;
	char	*old_pwd;
	char	*now_pwd;

	flag = 0;
	old_pwd = ft_strdup(master->pwd);
	now_pwd = ft_strdup(pwd);
	if (old_pwd == NULL || now_pwd == NULL)
		return (1);
	free(master->old_pwd);
	master->old_pwd = ft_strdup(old_pwd);
	master->pwd = ft_strdup(now_pwd);
	if (master->pwd == NULL || master->old_pwd == NULL)
		flag = 1;
	if (!((blt_search_and_update_env(env, "OLDPWD", old_pwd) == MS_BLT_SUCC) && \
		blt_search_and_update_env(env, "PWD", now_pwd) == MS_BLT_SUCC))
		flag = 1;
	return (flag);
}

// tree->token に引数が入ってる
// PWD, OLDPWDが unsetされている場合は新しく作らない
// 移動前のpwdはmaster->pwd
int	blt_cd_update_pwd(t_master *master, t_stree *tree, t_shellvar *env, int is_succ)
{
	char	*pwd;
	int		flag;

	errno = 0;
	pwd = getcwd(NULL, 0); // 移動後
	if (errno != 0)
	{
		if (tree == NULL)
			return (MS_BLT_FAIL);
		return (blt_cd_no_current(master, env, tree->token));
	}
	else if (tree) // 引数があった場合
	{
		if (is_succ)
		{
			free(pwd);
			pwd = blt_cd_has_args(master, tree);
		}
	}
	flag = blt_cd_update(master, env, pwd);
	free(pwd);
	return (flag);
}
