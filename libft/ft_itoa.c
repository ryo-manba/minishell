/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 22:32:20 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/04/10 15:16:22 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_size(long num)
{
	size_t	sz;

	sz = 1;
	if (num < 0)
	{
		num = -num;
		sz++;
	}
	while (num > 9)
	{
		num /= 10;
		sz++;
	}
	return (sz);
}

static char	*create_digit(char *convert_num, long num, size_t sz)
{
	int	flag;

	flag = 0;
	convert_num[sz] = '\0';
	while (sz--)
	{
		if (num < 0)
		{
			num = -num;
			flag = 1;
		}
		convert_num[sz] = num % 10 + '0';
		num /= 10;
	}
	if (flag)
		*convert_num = '-';
	return (convert_num);
}

char	*ft_itoa(int n)
{
	size_t	sz;
	long	num;
	char	*convert_num;

	num = (long)n;
	sz = get_size(num);
	convert_num = (char *)malloc((sizeof(char) * sz) + 1);
	if (convert_num == NULL)
		return (NULL);
	convert_num = create_digit(convert_num, num, sz);
	return (convert_num);
}
