/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd_no_current.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 15:28:25 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/18 15:40:45 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

char	*blt_check_slash_join(char *env_pwd, char *arg)
{
	size_t	sz;
	char	*tmp;
	char	*pwd;

	sz = ft_strlen(env_pwd);
	if (env_pwd[sz - 1] != '/')
	{
		tmp = ft_strjoin(env_pwd, "/");
		if (tmp == NULL)
		{
			ms_perror("malloc");
			return (NULL);
		}
		pwd = ft_strjoin(tmp, arg);
	}
	else
		pwd = ft_strjoin(env_pwd, arg);
	if (pwd == NULL)
	{
		ms_perror("malloc");
		return (NULL);
	}
	return (pwd);
}

int	blt_cd_no_current(t_shellvar *env, char *arg)
{
	t_shellvar	*pwd_pos;
	char		*pwd;
	int			flag;

	flag = 0;
	ft_putendl_fd("cd: error retrieving current directory: \
getcwd: cannot access parent directories: \
No such file or directory", STDERR_FILENO);
	pwd_pos = ms_search_key(env, "PWD");
	if (pwd_pos == NULL)
		return (MS_BLT_SUCC);
	pwd = blt_check_slash_join(pwd_pos->value, arg);
	if (pwd == NULL)
		return (MS_BLT_FAIL);
	flag = blt_cd_update(env, pwd_pos->value, pwd);
	free(pwd);
	return (flag);
}

int	blt_cd_no_prevdir(t_shellvar *env, char *now_pwd)
{
	t_shellvar	*pwd_pos;
	char		*old_pwd;

	pwd_pos = ms_search_key(env, "PWD");
	if (pwd_pos == NULL)
		return (MS_BLT_SUCC);
	old_pwd = ft_strdup(pwd_pos->value);
	if (old_pwd == NULL)
	{
		ms_perror("malloc");
		return (MS_BLT_FAIL);
	}
	if (blt_cd_update(env, old_pwd, now_pwd) == MS_BLT_FAIL)
		return (MS_BLT_FAIL);
	return (MS_BLT_SUCC);
}
