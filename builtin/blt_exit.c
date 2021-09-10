/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 17:48:50 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/06 18:00:00 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

// statusが LONG_MAX or LONG_MIN を超える場合
int	blt_check_long_overflow(char *ex_status)
{
	int			i;
	int			sign;
	uint64_t	unum;

	i = 0;
	sign = 1;
	unum = 0;
	if (ex_status[i] == '-' || ex_status[i] == '+')
	{
		if (ex_status[i++] == '-')
			sign = -1;
	}
	while (ft_isdigit(ex_status[i]))
	{
		unum = unum * 10 + ex_status[i] - '0';
		i++;
		if (unum != 0 && ((sign == 1 && unum > LONG_MAX)
				|| (sign == -1 && unum - 1 > LONG_MAX)))
			return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

/**
 * $ exit 1 2
 * tree->token = 1
 * tree->token->right = 2
 */
int	blt_exit(t_stree *tree)
{
	int		flag;
	int64_t	ex_status;

	if (tree == NULL)
		exit(0);
	if (blt_is_args_correct(tree->token) == MS_BLT_FAIL
		|| blt_check_long_overflow(tree->token) == MS_BLT_FAIL)
	{
		blt_exit_print_error(NOT_A_NUMBER, tree->token);
		exit(2);
	}
	if (tree->right != NULL)
	{
		blt_exit_print_error(TOO_MANY_ARGS, tree->token);
		return (MS_BLT_FAIL);
	}
	ex_status = ft_atoi(tree->token);
	ft_putendl_fd("exit", STDERR_FILENO);
	exit(ex_status);
}

void	blt_exit_print_error(int flag, char *error_args)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	if (flag == NOT_A_NUMBER)
	{
		ft_putstr_fd(error_args, STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	}
	if (flag == 2)
	{
		ft_putendl_fd("too many artuments", STDERR_FILENO);
	}
}

/**
 * $ exit "   42    "  OK
 * $ exit "   42 1  "  NG
 */
int	blt_is_args_correct(char *args)
{
	size_t	i;

	i = 0;
	while (ft_isspace(args[i]))
		i++;
	if (args[i] == '\0')
		return (MS_BLT_FAIL);
	while (ft_isdigit(args[i]))
		i++;
	while (ft_isspace(args[i]))
		i++;
	if (args[i] == '\0')
		return (MS_BLT_SUCC);
	return (MS_BLT_FAIL);
}