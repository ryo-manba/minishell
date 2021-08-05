/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 09:54:43 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/04/14 09:30:52 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define LLONG_MAX 9223372036854775807L

static long	over_num(int flag)
{
	if (flag == 1)
		return (-1);
	else
		return (0);
}

static int	is_space(char c)
{
	if (c == ' ')
		return (1);
	if (c == '\n')
		return (1);
	if (c == '\t')
		return (1);
	if (c == '\r')
		return (1);
	if (c == '\v')
		return (1);
	if (c == '\f')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int		flag;
	long	num;
	long	ov_div;
	long	ov_mod;

	num = 0;
	flag = 1;
	ov_div = LLONG_MAX / 10;
	ov_mod = LLONG_MAX % 10;
	while (is_space(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str++ == '-')
			flag = -1;
	}
	while (*str >= '0' && *str <= '9')
	{
		if (num > ov_div || (num == ov_div && *str - '0' > ov_mod))
			return (over_num(flag));
		num = num * 10 + *str - '0';
		str++;
	}
	num *= flag;
	return (num);
}
