/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_check_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/15 15:32:33 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/23 18:00:23 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

// '/' が入っていてstatで存在しない場合はn
// 実行可能ビットが立ってたらOK (S_IXUSR (00100))所有者による実行
// 絶対パスの場合
int	exec_check_path(char *path, int is_relative)
{
	struct stat	sb;

	if (is_relative)
		return (exec_check_relative(path));
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

// 相対パス
int	exec_check_relative(char *path)
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
			return (MS_EXEC_SUCC);
		else
			g_ex_states = PERMISSION;
	}
	return (MS_EXEC_FAIL);
}
