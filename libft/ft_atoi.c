/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 09:54:43 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/03 13:57:03 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long	over_num(int sign)
{
	if (sign == 1)
		return (-1);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	int		sign;
	long	num;
	long	ov_div;
	long	ov_mod;

	num = 0;
	sign = 1;
	ov_div = LONG_MAX / 10;
	ov_mod = LONG_MAX % 10;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str++ == '-')
			sign = -1;
	}
	while ('0' <= *str && *str <= '9')
	{
		if (num > ov_div || (num == ov_div && *str - '0' > ov_mod))
			return (over_num(sign));
		num = num * 10 + *str - '0';
		str++;
	}
	num *= sign;
	return (num);
}
