/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd_update_pwd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 15:29:59 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/19 20:41:39 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

int	blt_cd_update(t_shellvar *env, char *old_pwd, char *now_pwd)
{
	if ((blt_search_and_update_env(env, "OLDPWD", old_pwd) == MS_BLT_FAIL) || \
		blt_search_and_update_env(env, "PWD", now_pwd) == MS_BLT_FAIL)
		return (MS_BLT_FAIL);
	return (MS_BLT_SUCC);
}

// PWD, OLDPWDが unsetされている場合は新しく作らない
int	blt_cd_update_pwd(t_shellvar *env, t_stree *tree, char *old_pwd)
{
	char	*pwd;
	int		flag;

	flag = 0;
	errno = 0;
	pwd = getcwd(NULL, 0);
	if (errno != 0)
	{
		if (tree == NULL)
			return (MS_BLT_FAIL);
		return (blt_cd_no_current(env, tree->token));
	}
	else if (old_pwd == NULL)
		flag = blt_cd_no_prevdir(env, pwd);
	else
		flag = blt_cd_update(env, old_pwd, pwd);
	free(pwd);
	if (flag)
		return (MS_BLT_FAIL);
	return (MS_BLT_SUCC);
}
