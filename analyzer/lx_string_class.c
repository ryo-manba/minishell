/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_string_class.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:28:02 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/27 01:54:44 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

int	lx_str_is_digital(const	char *str, size_t len)
{
	size_t	n;

	n = 0;
	while (n < len && str[n])
		if (!ft_isdigit(str[n++]))
			return (0);
	return (n > 0);
}

int	lx_str_is_for_name(const char *str, size_t len)
{
	size_t	n;

	n = 0;
	while (n < len && str[n])
	{
		if (n == 0 && !ft_isalpha(str[n]) && str[n] != '_')
			return (0);
		else if (n > 0 && !ft_isalnum(str[n]) && str[n] != '_')
			return (0);
		n += 1;
	}
	return (n > 0);
}

int	lx_str_is_for_assignment_word(const char *str, size_t len)
{
	int		bs[2];
	int		bbs;
	size_t	n;

	bs[0] = 0;
	bs[1] = 0;
	bbs = 0;
	n = 0;
	while (n < len && str[n])
	{
		if (!bs[n % 2] && str[n] == '=')
		{
			if (n > 0 && !bbs && str[n - 1] == '+')
				return (lx_str_is_for_name(str, n - 1));
			return (lx_str_is_for_name(str, n));
		}
		bbs = bs[n % 2];
		bs[(n + 1) % 2] = str[n] == '\\' && !bs[n % 2];
		n += 1;
	}
	return (0);
}
