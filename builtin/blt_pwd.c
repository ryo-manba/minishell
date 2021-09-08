/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 18:00:15 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/08 17:24:28 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

int	blt_pwd(void)
{
	char	*path;

	errno = 0;
	path = getcwd(NULL, 0);
	if (errno != 0)
	{
		ms_print_perror("getcwd");
		return (MS_BLT_FAIL);
	}
	ft_putendl_fd(path, STDOUT_FILENO);
	free(path);
	return (MS_BLT_SUCC);
}
