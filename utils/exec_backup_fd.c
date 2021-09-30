/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_backup_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 00:08:03 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/30 21:52:06 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

int	exec_close_backup_fd(int backup_fd[3])
{
	int	ret_zero;
	int	ret_one;
	int	ret_two;

	ret_zero = close(backup_fd[0]);
	ret_one = close(backup_fd[1]);
	ret_two = close(backup_fd[2]);
	if (ret_zero || ret_one || ret_two)
		return (1);
	return (0);
}

// Make a backup of fd(0,1,2) for redirection in the parent process.
int	exec_create_backup_fd(int backup_fd[3])
{
	backup_fd[0] = dup(STDIN_FILENO);
	backup_fd[1] = dup(STDOUT_FILENO);
	backup_fd[2] = dup(STDERR_FILENO);
	if (backup_fd[0] == -1 || backup_fd[1] == -1 || backup_fd[2] == -1)
	{
		ms_perror("dup");
		return (1);
	}
	return (0);
}

int	exec_duplicate_backup_fd(int backup_fd[3])
{
	int	ret_zero;
	int	ret_one;
	int	ret_two;
	int	ret_close;

	ret_zero = dup2(backup_fd[0], STDIN_FILENO);
	ret_one = dup2(backup_fd[1], STDOUT_FILENO);
	ret_two = dup2(backup_fd[2], STDERR_FILENO);
	ret_close = exec_close_backup_fd(backup_fd);
	if (ret_zero == -1 || ret_one == -1 || ret_two == -1 || ret_close)
		return (1);
	return (0);
}
