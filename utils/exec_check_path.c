/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_check_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 21:27:00 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/27 10:54:38 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

static int	exec_check_cmd_stat(char *path)
{
	struct stat	sb;
	int			rv;

	rv = stat(path, &sb);
	if (rv < 0 || S_ISDIR(sb.st_mode))
	{
		g_ex_states = CMD_NOT_FOUND;
		return (MS_EXEC_FAIL);
	}
	if (S_ISREG(sb.st_mode))
	{
		if ((sb.st_mode & S_IXUSR))
		{
			g_ex_states = 0;
			return (MS_EXEC_SUCC);
		}
		else
			g_ex_states = PERMISSION;
	}
	return (MS_EXEC_FAIL);
}

int	exec_check_path_stat(char *path, int is_command)
{
	struct stat	sb;

	if (is_command)
		return (exec_check_cmd_stat(path));
	if (stat(path, &sb) < 0)
	{
		g_ex_states = NO_SUCH_FILE;
		return (MS_EXEC_FAIL);
	}
	if (S_ISDIR(sb.st_mode))
	{
		g_ex_states = IS_A_DIR;
		return (MS_EXEC_FAIL);
	}
	if (S_ISREG(sb.st_mode))
	{
		if ((sb.st_mode & S_IXUSR) && (sb.st_mode & S_IRUSR))
			return (MS_EXEC_SUCC);
		g_ex_states = PERMISSION;
	}
	return (MS_EXEC_FAIL);
}
