/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpush_back.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/23 21:57:34 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/15 23:33:31 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstpush_back(t_list **lst, void *content)
{
	t_list	*newlst;

	newlst = ft_lstnew(content);
	if (!newlst)
		return (1);
	ft_lstadd_back(lst, newlst);
	return (0);
}
