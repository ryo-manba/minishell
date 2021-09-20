/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 13:26:37 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/20 14:31:45 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

void	exec_error_prologue(t_master *master, int limit_line)
{
	ft_putstr_fd(master->prog_name, STDERR_FILENO);
	if (!master->interactive_shell)
	{
		if (!limit_line || master->line_num > 0)
		{
			ft_putstr_fd(": line ", STDERR_FILENO);
			ft_putsizet_fd(master->line_num, STDERR_FILENO);
		}
	}
	ft_putstr_fd(": ", STDERR_FILENO);
}

void	exec_print_error_exit(t_master *master, int ex_status, char *path)
{
	if (ex_status == CMD_NOT_FOUND && master->line_num == 0)
		master->interactive_shell = 1;
	exec_error_prologue(master, 1);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	free(path);
	if (ex_status == IS_A_DIR || ex_status == PERMISSION)
	{
		if (ex_status == IS_A_DIR)
			ft_putendl_fd("is a directory", STDERR_FILENO);
		if (ex_status == PERMISSION)
			ft_putendl_fd("Permission denied", STDERR_FILENO);
		exit(126);
	}
	else if (ex_status == CMD_NOT_FOUND)
		ft_putendl_fd("command not found", STDERR_FILENO);
	else if (ex_status == NO_SUCH_FILE)
		ft_putendl_fd("No such file or directory", STDERR_FILENO);
	exit(127);
}

void	exec_print_prologue(t_master *master)
{
	ft_putstr_fd(master->prog_name, STDERR_FILENO);
	ft_putstr_fd(": warning: ", STDERR_FILENO);
}
