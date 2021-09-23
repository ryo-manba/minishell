/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_rough.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 19:03:27 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/23 20:49:54 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_split_more_rec(char const *str, char c, size_t i, size_t n)
{
	size_t	j;
	char	**list;

	j = i;
	while (str[j] && str[j] != c)
		j += 1;
	if (!str[j])
	{
		list = (char **)malloc(sizeof(char *) * (n + 1));
		if (list)
			list[n] = NULL;
	}
	else
		list = ft_split_more_rec(str, c, j + 1, n + 1);
	if (!list)
		return (NULL);
	list[n - 1] = ft_substr(str, i, j - i);
	if (list[n - 1])
		return (list);
	while (list[n])
		free(list[n++]);
	free(list);
	return (NULL);
}

// like a ft_split, but returns ""(blank string).
char	**ft_split_rough(char const *str, char c)
{
	return (ft_split_more_rec(str, c, 0, 1));
}
