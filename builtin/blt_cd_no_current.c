/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd_no_current.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 15:28:25 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/10/01 17:17:33 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

static char	*blt_check_slash_join(char *env_pwd, char *arg)
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
		free(tmp);
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

int	blt_cd_no_current(t_master *master, t_shellvar *env, char *arg)
{
	char		*pwd;
	int			ret;

	ft_putendl_fd("cd: error retrieving current directory: \
getcwd: cannot access parent directories: \
No such file or directory", STDERR_FILENO);
	if (master->pwd == NULL)
		return (MS_BLT_SUCC);
	pwd = blt_check_slash_join(master->pwd, arg);
	if (pwd == NULL)
		return (MS_BLT_FAIL);
	ret = blt_cd_update(master, env, pwd);
	free(pwd);
	return (ret);
}
