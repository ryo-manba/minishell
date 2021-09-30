/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 20:43:42 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/30 19:53:39 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	chmin(size_t *n1, size_t n2)
{
	if (*n1 > n2)
		*n1 = n2;
}

char	*ft_substr(char const *s, size_t start, size_t len)
{
	char	*str;
	size_t	i;
	size_t	s_len;

	if (s == NULL)
		return (NULL);
	s_len = ft_strlen(s);
	if (s_len <= start)
		len = 0;
	chmin(&len, (s_len - start + 1));
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (len--)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_substr_range(char const *s, unsigned int start, unsigned int end)
{
	if (start <= end)
		return (ft_substr(s, start, end - start));
	return (NULL);
}
