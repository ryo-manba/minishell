/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 13:26:14 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/19 11:33:34 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

int	ms_exec_builtin(t_stree *tree, t_master *master)
{
	if (ft_strcmp(tree->token, "cd") == 0)
		return (blt_cd(master->var, tree->right, master));
	if (ft_strcmp(tree->token, "echo") == 0)
		return (blt_echo(tree->right, master));
	if (ft_strcmp(tree->token, "env") == 0)
		return (blt_env(master->var));
	if (ft_strcmp(tree->token, "exit") == 0)
		return (blt_exit(tree->right, master));
	if (ft_strcmp(tree->token, "export") == 0)
		return (blt_export(master->var, tree->right, master));
	if (ft_strcmp(tree->token, "pwd") == 0)
		return (blt_pwd(master->var));
	if (ft_strcmp(tree->token, "unset") == 0)
		return (blt_unset(master, tree->right));
	return (0);
}

int	ms_is_builtin(t_stree *tree)
{
	if (ft_strcmp(tree->token, "cd") == 0)
		return (1);
	if (ft_strcmp(tree->token, "echo") == 0)
		return (1);
	if (ft_strcmp(tree->token, "env") == 0)
		return (1);
	if (ft_strcmp(tree->token, "exit") == 0)
		return (1);
	if (ft_strcmp(tree->token, "export") == 0)
		return (1);
	if (ft_strcmp(tree->token, "pwd") == 0)
		return (1);
	if (ft_strcmp(tree->token, "unset") == 0)
		return (1);
	return (0);
}

void	ms_perror(char *func_name)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(func_name);
}

void	ms_perror_exit(char *func_name)
{
	ms_perror(func_name);
	exit(1);
}
