#include "ms_builtin.h"

void	blt_exit_print_error(int flag, char *error_args)
{
	ft_putendl_fd("exit", STDOUT_FILENO);
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	if (flag == NOT_A_NUMBER) // 数字以外, またはLONGでオーバーフローする場合
	{
		ft_putstr_fd(error_args, STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	}
	if (flag == 2) // 数字が続いた場合 exitしない
	{
		ft_putendl_fd("too many artuments", STDERR_FILENO);
	}
}

/**
 * $ exit "   42    "  OK
 * $ exit "   42 1  "  NG
 * $ exit "   42 a  "  NG
 */
// 引数が正しいか
int	blt_is_args_correct(char *args)
{
	size_t	i;

	i = 0;
	while (ft_isspace(args[i]))
		i++;
	while (ft_isdigit(args[i])) // 数字じゃなかったら
		i++;
	while (ft_isspace(args[i]))
		i++;
	if (args[i] == '\0')
		return (MS_BLT_SUCC);
	return (MS_BLT_FAIL);
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
		if (unum != 0 && ((sign == 1 && unum > LONG_MAX)
			|| (sign == -1 && unum - 1 > LONG_MAX)))
			return (MS_BLT_FAIL); // 上限を超えた場合
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
	int64_t	status;

	if (tree == NULL) // 引数なしはそのままexit
		exit(0);
	if (blt_is_args_correct(tree->token) == MS_BLT_FAIL
		|| blt_check_long_overflow(tree->token) == MS_BLT_FAIL) // 数字以外、またはオーバーフロー
	{
		blt_exit_print_error(NOT_A_NUMBER, tree->token); // 第一引数エラーはエラーメッセージを出してexitする
		exit(2);
	}
	if (tree->right != NULL) // 引数が2つ以上ある場合、エラーメッセージ出してexitしない
	{
		blt_exit_print_error(TOO_MANY_ARGS, tree->token);
		return (MS_BLT_FAIL);
	}
	status = ft_atoi(tree->token);
	ft_putendl_fd("exit", STDOUT_FILENO);
	exit(status);
}
