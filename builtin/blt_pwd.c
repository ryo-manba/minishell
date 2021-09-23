/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 18:00:15 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/23 20:37:36 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

int	blt_pwd(t_master *master)
{
	if (master->pwd == NULL)
	{
		ft_putendl_fd("pwd: error retrieving current directory: \
getcwd: cannot access parent directories: \
No such file or directory", STDERR_FILENO);
		return (MS_BLT_FAIL);
	}
	ft_putendl_fd(master->pwd, STDOUT_FILENO);
	return (MS_BLT_SUCC);
}
