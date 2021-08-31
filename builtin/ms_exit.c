#include "ms_builtin.h"

void	blt_exit_print_error(int flag, char *error_args)
{
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	if (flag == 1) // 数字以外, またはLONGでオーバーフローする場合
	{
		ft_putstr_fd(error_args, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	}
	if (flag == 2) // 数字が続いた場合 exitしない
	{
		ft_putstr_fd("too many artuments\n", STDERR_FILENO);
	}
}

int	blt_is_args_digit(char *args)
{
	size_t	i;

	i = 0;
	while (args[i])
	{
		if (ft_isdigit(args[i]) == 0) // 数字じゃなかったら
			return (1);
		i++;
	}
	return (0);
}

// statusが LONG_MAX or LONG_MIN を超える場合
int	blt_check_long_overflow(char *status)
{
	int			i;
	int			sign;
	uint64_t	unum;

	i = 0;
	sign = 1;
	unum = 0;
	if (status[i] == '-' || status[i] == '+')
	{
		if (status[i++] == '-')
			sign = -1;
	}
	while (ft_isdigit(status[i]))
	{
		unum = unum * 10 + status[i] - '0';
		i++;
		if (unum != 0 && (sign == 1 && unum > LONG_MAX)
			|| (sign == -1 && unum - 1 > LONG_MAX))
			return (1); // 上限を超えた場合
	}
	return (0);
}

/**
 * $ exit 1 2
 * tree->token = 1
 * tree->token->right = 2
 */
int	blt_exit(t_stree *tree)
{
	int flag;
	int64_t	status;

	if (tree == NULL) // 引数なしはそのままexit
		exit(0);
	if (blt_is_args_digit(tree->token) == 1
		|| blt_check_long_overflow(tree->token) == 1) // 数字以外、またはオーバーフロー
	{
		blt_exit_print_error(1, tree->token); // 第一引数エラーはエラーメッセージを出してexitする
		exit(2);
	}
	if (tree->right != NULL) // 引数が2つ以上ある場合、エラーメッセージ出してexitしない
	{
		blt_exit_print_error(2, tree->token);
		return (1);
	}
	status = ft_atoi(tree->token);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(status);
}
