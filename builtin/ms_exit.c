#include "ms_builtin.h"

#define TOO_MANY_ARGS 1
#define NOT_A_NUMBER 2
#define OVERFLOW 3

// -2147483648->0
// 2147483647->255

static int64_t	ms_argument_error(int flag, char *error_args)
{
	if (flag == NOT_A_NUMBER || flag == OVERFLOW) // 数字以外, またはLONGでオーバーフローする場合
	{
		printf("minishell: exit: %s: numeric argument required\n", error_args);
		exit(2);
	}
	if (flag == TOO_MANY_ARGS) // 数字が続いた場合 exitしない
	{
		printf("minishell: exit: too many artuments\n");
	}
	return (1);
}

// statusが LONG_MAX or LONG_MIN を超える場合
static int64_t	ms_create_and_check_status(char *status)
{
	uint64_t	unum;
	int64_t		ret;
	int	i;
	int	sign;

	sign = 1;
	unum = 0;
	i = 0;
	if (status[i] == '-' || status[i] == '+')
	{
		if (status[i++] == '-')
			sign = -1;
	}
	while ('0' <= status[i] && status[i] <= '9')
	{
		unum = unum * 10 + status[i] - '0';
		i++;
		if (unum != 0 && (sign == 1 && unum > LONG_MAX) || (sign == -1 && unum - 1 > LONG_MAX))
			return (OVERFLOW); // 上限を超えた場合
	}
	if (ft_strlen(status) != i) // 数字以外の文字が入っている場合
		return (NOT_A_NUMBER);
	if (false) // 第一引数が正しい値で第二引数以上含まれている場合 [後で足す]
		return (TOO_MANY_ARGS);
	return (0);
}

// exit -- 42
// exitの後の -- は無視される
int	ms_exit(char *arg)
{
	int flag;
	int64_t	status;

	flag = ms_create_and_check_status(arg);
	printf("%d\n", flag);
	if (flag != 0)
		ms_argument_error(flag, arg);
	else
	{
		status = ft_atoi(arg);
		printf("exit\n");
		exit(status);
	}
	return (0);
}
