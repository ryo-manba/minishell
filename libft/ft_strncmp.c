/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 13:24:44 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/04/10 17:57:40 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*s1_ptr;
	unsigned char	*s2_ptr;

	s1_ptr = (unsigned char *)s1;
	s2_ptr = (unsigned char *)s2;
	while (*s1_ptr == *s2_ptr)
	{
		if (*s1_ptr && *s2_ptr && n > 0)
		{
			s1_ptr++;
			s2_ptr++;
			n--;
		}
		else
			return (*s1_ptr - *s2_ptr);
	}
	if (n == 0)
		return (0);
	return (*s1_ptr - *s2_ptr);
}
