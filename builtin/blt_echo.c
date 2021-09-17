/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 14:53:20 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/17 12:36:56 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

/**
 * $ echo -n hello
 * tree->token = -n
 * tree->right->token = hello
 */
int	blt_echo(t_stree *tree)
{
	int	has_option;

	errno = 0;
	has_option = 0;
	while (tree != NULL && ft_strcmp(tree->token, "-n") == 0)
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
		blt_echo_print_error(strerror(errno));
		return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

void	blt_echo_print_error(char *message)
{
	ft_putstr_fd("minishell: echo: write error: ", STDERR_FILENO);
	ft_putendl_fd(message, STDERR_FILENO);
}
