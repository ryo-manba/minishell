/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_starts_with.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/13 02:59:33 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/13 02:59:38 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_starts_with(const char *heystack, const char *needle)
{
	size_t	n;

	n = 0;
	while (*(needle + n))
	{
		if (*(needle + n) != *(heystack + n))
			return (0);
		n += 1;
	}
	return (n);
}
