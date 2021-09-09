/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 13:26:37 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/09 14:19:26 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

void	exec_print_error_exit(int ex_status, char *path)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (ex_status == IS_A_DIR || ex_status == PERMISSION)
	{
		if (ex_status == IS_A_DIR)
			ft_putendl_fd("is a directory", STDERR_FILENO);
		if (ex_status == PERMISSION)
			ft_putendl_fd("Permission denied", STDERR_FILENO);
		exit(126);
	}
	else if (ex_status == CMD_NOT_FOUND)
	{
		ft_putendl_fd("command not found", STDERR_FILENO);
		exit(127);
	}
	else
	{
		ft_putendl_fd("No such file or directory", STDERR_FILENO);
		exit(127);
	}
}

void	exec_print_error(t_clause *clause)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(clause->stree->token, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
}
