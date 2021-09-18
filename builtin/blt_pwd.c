/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 18:00:15 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/18 15:25:13 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

int	blt_print_env_pwd(t_shellvar *var)
{
	t_shellvar	*pwd_pos;

	pwd_pos = ms_search_key(var, "PWD");
	if (pwd_pos == NULL)
		return (MS_BLT_FAIL);
	ft_putendl_fd(pwd_pos->value, STDOUT_FILENO);
	return (MS_BLT_SUCC);
}

int	blt_pwd(t_shellvar *var)
{
	char	*path;

	if (blt_print_env_pwd(var) == MS_BLT_SUCC)
		return (MS_BLT_SUCC);
	errno = 0;
	path = getcwd(NULL, 0);
	if (errno != 0)
	{
		ms_perror("getcwd");
		return (MS_BLT_FAIL);
	}
	ft_putendl_fd(path, STDOUT_FILENO);
	free(path);
	return (MS_BLT_SUCC);
}
