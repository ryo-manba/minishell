/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 13:26:14 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/08 17:26:00 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

int	ms_exec_builtin(t_shellvar *env, t_stree *tree)
{
	if (ft_strcmp(tree->token, "cd") == 0)
		return (blt_cd(env, tree->right));
	if (ft_strcmp(tree->token, "echo") == 0)
		return (blt_echo(tree->right));
	if (ft_strcmp(tree->token, "env") == 0)
		return (blt_env(env));
	if (ft_strcmp(tree->token, "exit") == 0)
		return (blt_exit(tree->right));
	if (ft_strcmp(tree->token, "export") == 0)
		return (blt_export(env, tree->right));
	if (ft_strcmp(tree->token, "pwd") == 0)
		return (blt_pwd());
	if (ft_strcmp(tree->token, "unset") == 0)
		return (blt_unset(env, tree->right));
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

void	ms_print_perror(char *func_name)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(func_name);
}
