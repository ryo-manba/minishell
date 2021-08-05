/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka < rmatsuka@student.42tokyo.jp>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/31 11:17:05 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/05/31 11:18:13 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char	*s1_ptr;
	unsigned char	*s2_ptr;

	s1_ptr = (unsigned char *)s1;
	s2_ptr = (unsigned char *)s2;
	while (*s1_ptr && *s2_ptr)
	{
		if (*s1_ptr == *s2_ptr)
		{
			s1_ptr++;
			s2_ptr++;
		}
		else
			return (*s1_ptr - *s2_ptr);
	}
	return (*s1_ptr - *s2_ptr);
}
