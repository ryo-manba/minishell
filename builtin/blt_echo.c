/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 14:53:20 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/19 21:17:58 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

int	blt_check_option(char *token)
{
	int	i;

	i = 1;
	if (ft_strlen(token) >= 2 && ft_strncmp(token, "-n", 2) == 0)
	{
		while (token[i])
		{
			if (token[i] != 'n')
				return (0);
			i += 1;
		}
		return (1);
	}
	return (0);
}

/**
 * $ echo -n hello
 * tree->token = -n
 * tree->right->token = hello
 */
int	blt_echo(t_stree *tree, t_master *master)
{
	int	has_option;

	errno = 0;
	has_option = 0;
	while (tree != NULL && blt_check_option(tree->token))
	{
		has_option = 1;
		tree = tree->right;
	}
	while (tree != NULL)
	{
		ft_putstr_fd(tree->token, STDOUT_FILENO);
		if (tree->right != NULL)
			ft_putchar_fd(' ', STDOUT_FILENO);
		tree = tree->right;
	}
	if (has_option == 0)
		ft_putchar_fd('\n', STDOUT_FILENO);
	if (errno != 0)
	{
		blt_echo_print_error(master, strerror(errno));
		return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

void	blt_echo_print_error(t_master *master, char *message)
{
	exec_error_prologue(master);
	ft_putstr_fd("echo: write error: ", STDERR_FILENO);
	ft_putendl_fd(message, STDERR_FILENO);
}
