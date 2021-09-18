/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd_update_pwd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 15:29:59 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/18 15:34:58 by rmatsuka         ###   ########.fr       */
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
int	blt_cd_update_pwd(t_shellvar *env, char *old_pwd, char *arg)
{
	char	*pwd;
	int		flag;

	flag = 0;
	errno = 0;
	pwd = getcwd(NULL, 0);
	if (errno != 0)
		return (blt_cd_no_current(env, arg));
	else if (old_pwd == NULL)
		flag = blt_cd_no_prevdir(env, pwd);
	else
		flag = blt_cd_update(env, old_pwd, pwd);
	free(pwd);
	if (flag)
		return (MS_BLT_FAIL);
	return (MS_BLT_SUCC);
}
