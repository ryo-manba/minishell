/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 07:59:10 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/04/08 10:34:56 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *dest, int value, size_t sz)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)dest;
	while (sz--)
	{
		*ptr = value;
		ptr++;
	}
	return (dest);
}
