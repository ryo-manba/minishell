/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd_lst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 12:26:20 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/30 19:59:48 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

t_list	*blt_lst_init(t_list **del_lst)
{
	t_list	*lst;

	ft_lstclear(del_lst, free);
	lst = (t_list *)ft_calloc(sizeof(t_list), 1);
	return (lst);
}

int	blt_malloc_failed_lstclear(t_list **lst)
{
	ft_lstclear(lst, free);
	ms_perror("malloc");
	return (MS_BLT_FAIL);
}

static int	blt_create_lst_loop(t_list **lst, char *s, size_t idx)
{
	size_t	len;
	size_t	start;
	char	*dir;

	while (s[idx])
	{
		len = 0;
		start = idx;
		while (s[idx] && s[idx] != '/')
		{
			idx += 1;
			len += 1;
		}
		if (start != idx)
		{
			dir = ft_substr(s, start, len);
			if (dir == NULL)
				return (blt_malloc_failed_lstclear(lst));
			ft_lstpush_back(lst, dir);
		}
		while (s[idx] == '/')
			idx += 1;
	}
	return (MS_BLT_SUCC);
}

t_list	*blt_cd_create_list(char *s, int is_absolute)
{
	size_t	i;
	t_list	*lst;

	lst = (t_list *)ft_calloc(1, sizeof(t_list));
	if (lst == NULL)
	{
		ms_perror("malloc");
		return (NULL);
	}
	i = 0;
	if (is_absolute)
	{
		i = blt_pre_absolute_lst(&lst, s);
		if (i == 0)
			return (lst);
	}
	blt_create_lst_loop(&lst, s, i);
	return (lst);
}

void	blt_lstlast_del(t_list *lst)
{
	t_list	*last;
	t_list	*prev;

	last = lst;
	prev = NULL;
	if (last->next == NULL)
		return ;
	while (last->next)
	{
		prev = last;
		last = last->next;
	}
	if (prev)
	{
		prev->next = NULL;
		ft_lstdelone(last, free);
	}
}
