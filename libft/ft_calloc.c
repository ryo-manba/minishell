/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 11:58:10 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/13 03:00:12 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t n, size_t sz)
{
	void	*ptr;
	size_t	value;

	if (n == 0 || sz == 0)
	{
		n = 1;
		sz = 1;
	}
	value = n * sz;
	ptr = (void *)malloc(value);
	if (ptr == NULL)
		return (NULL);
	ft_memset(ptr, 0, value);
	return (ptr);
}
