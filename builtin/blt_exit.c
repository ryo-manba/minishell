/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 17:48:50 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/30 20:10:19 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

// return 2 with NOT_A_NUMBER(for newer bash)
static int	blt_exit_print_error(t_master *master, int flag, char *error_args)
{
	if (master->interactive_shell)
		ft_putendl_fd("exit", STDERR_FILENO);
	exec_error_prologue(master, 0);
	ft_putstr_fd("exit: ", STDERR_FILENO);
	if (flag == NOT_A_NUMBER)
	{
		ft_putstr_fd(error_args, STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		return (2);
	}
	if (flag == 2)
	{
		ft_putendl_fd("too many arguments", STDERR_FILENO);
		return (1);
	}
	return (MS_BLT_FAIL);
}

/**
 * $ exit "   42    "  OK
 * $ exit "   42 1  "  NG
 */
static int	blt_is_args_correct(char *args)
{
	size_t	i;

	i = 0;
	if (args[i] == '+' || args[i] == '-')
		i += 1;
	while (ft_isspace(args[i]))
		i += 1;
	if (args[i] == '\0')
		return (MS_BLT_FAIL);
	while (ft_isdigit(args[i]))
		i += 1;
	while (ft_isspace(args[i]))
		i += 1;
	if (args[i] == '\0')
		return (MS_BLT_SUCC);
	return (MS_BLT_FAIL);
}

static int	blt_check_long_overflow(char *ex_status)
{
	int			sign;
	size_t		i;
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
		if ((sign == 1 && unum > LONG_MAX) || \
			(sign == -1 && unum > (uint64_t)LONG_MAX + 1ULL))
			return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

/**
 * $ exit 1 2
 * tree->token = 1
 * tree->token->right = 2
 */
int	blt_exit(t_stree *tree, t_master *master)
{
	int64_t	ex_status;

	master->exited = 1;
	if (tree == NULL)
		exit(g_ex_states);
	if (blt_is_args_correct(tree->token) == MS_BLT_FAIL || \
		blt_check_long_overflow(tree->token) == MS_BLT_FAIL)
		exit(blt_exit_print_error(master, NOT_A_NUMBER, tree->token));
	if (tree->right != NULL)
		return (blt_exit_print_error(master, TOO_MANY_ARGS, tree->token));
	ex_status = ft_atoi(tree->token);
	if (master->interactive_shell)
		ft_putendl_fd("exit", STDERR_FILENO);
	exit(ex_status);
}
