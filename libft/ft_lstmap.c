/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/11 14:53:35 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/04/13 20:29:53 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*map_lst;
	t_list	*next_lst;
	t_list	*next_back;

	if (lst == NULL)
		return (NULL);
	map_lst = ft_lstnew((*f)(lst->content));
	if (map_lst == NULL)
		return (NULL);
	next_lst = map_lst;
	next_back = lst->next;
	while (lst)
	{
		if (next_back == NULL)
			break ;
		next_lst->next = ft_lstnew((*f)(next_back->content));
		if (next_lst->next == NULL)
		{
			ft_lstclear(&map_lst, (*del));
			return (NULL);
		}
		next_lst = next_lst->next;
		next_back = next_back->next;
	}
	return (map_lst);
}
