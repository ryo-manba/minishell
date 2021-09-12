/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_string_class.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:28:02 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/13 00:02:07 by yokawada         ###   ########.fr       */
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

int	lx_char_is_for_name(char c, size_t pos)
{
	if (pos == 0 && !ft_isalpha(c) && c != '_')
		return (0);
	else if (pos > 0 && !ft_isalnum(c) && c != '_')
		return (0);
	return (1);
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

int	lx_char_is_for_bare(char c)
{
	if (!c)
		return (0);
	return (!ft_strchr("'\"$", c));
}

// strがASSIGNMENT WORDとして適当かどうか?
// 1. strが文字'='を含むこと
// 2. strの文字'='より前の部分がNAMEとして適当であること
//   ただし、'='の左隣に'+'がある場合、それは除外して判定する。
int	lx_str_is_for_assignment_word(const char *str, size_t len)
{
	size_t	n;

	n = 0;
	while (n < len && str[n])
	{
		if (str[n] == '=')
		{
			if (n > 0 && str[n - 1] == '+')
				return (lx_str_is_for_name(str, n - 1));
			return (lx_str_is_for_name(str, n));
		}
		n += 1;
	}
	return (0);
}
