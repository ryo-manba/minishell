#include "ms_builtin.h"

void	ms_exit_argument_error(int flag, char *error_args)
{
	printf("exit\n");
	if (flag == 1) // 数字以外, またはLONGでオーバーフローする場合
	{
		printf("minishell: exit: %s: numeric argument required\n", error_args);
	}
	if (flag == 2) // 数字が続いた場合 exitしない
	{
		printf("minishell: exit: too many artuments\n");
	}
}

int	ms_is_args_digit(char *args)
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
int	ms_check_long_overflow(char *status)
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
int	ms_exit(t_stree *tree)
{
	int flag;
	int64_t	status;

	if (tree == NULL) // 引数なしはそのままexit
		exit(0);
	if (ms_is_args_digit(tree->token) == 1 || ms_check_long_overflow(tree->token) == 1) // 数字以外、またはオーバーフローする
	{
		ms_exit_argument_error(1, tree->token); // 第一引数エラーはエラーメッセージ出してexitする
		exit(2);
	}
	if (tree->right != NULL) // 引数が2つ以上ある場合、エラーメッセージ出してexitしない
	{
		ms_exit_argument_error(2, tree->token);
		return (1);
	}
	status = ft_atoi(tree->token);
	printf("exit\n");
	exit(status);
}
