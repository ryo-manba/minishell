/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpush_back.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka < rmatsuka@student.42tokyo.jp>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/23 21:57:34 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/07/23 22:08:05 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_lstpush_back(t_list **lst, void *content)
{
	t_list	*newlst;

	newlst = ft_lstnew(content);
	if (!newlst)
		return (false);
	ft_lstadd_back(lst, newlst);
	return (true);
}
