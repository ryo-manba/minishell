/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mf_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 18:59:06 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/15 09:22:25 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mf_print_errno(t_master *master, int mark, int given_errno)
{
	if (mark)
	{
		ft_putstr_fd(master->prog_name, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		if (master->filepath)
		{
			ft_putstr_fd(master->filepath, STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
		}
		ft_putstr_fd(strerror(given_errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		master->failed = mark;
	}
	return (master->failed);
}

int	mf_mark_failed(t_master *master, int mark, char *message)
{
	if (mark)
	{
		master->failed = mark;
		ft_putstr_fd(master->prog_name, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		if (!message && errno)
			ft_putstr_fd(strerror(errno),
				STDERR_FILENO);
		else if (!message)
			ft_putstr_fd("error in init",
				STDERR_FILENO);
		else
			ft_putstr_fd(message, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	return (master->failed);
}
