/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 20:11:06 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/04/09 14:13:30 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	if (*needle == '\0')
		return ((char *)haystack);
	if (len == 0)
		return (NULL);
	i = 0;
	j = 0;
	while (1)
	{
		if (needle[i] == '\0')
			return ((char *)(haystack + (j - i)));
		if (haystack[j] == needle[i])
			i++;
		else
		{
			j -= i;
			i = 0;
		}
		if (haystack[j] == '\0' || j >= len)
			return (NULL);
		j++;
	}
}
