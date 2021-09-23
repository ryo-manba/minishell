/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd_normalize_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka < rmatsuka@student.42tokyo.jp>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 12:31:03 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/23 14:10:40 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

void	blt_pre_absolute_path(t_list **pwd_lst, t_list **arg_lst)
{
	*pwd_lst = blt_lst_init(pwd_lst);

	if (*arg_lst && (ft_strlen((*arg_lst)->content) == 2 && \
		ft_strcmp((*arg_lst)->content, "//") == 0))
	{
		ft_lstpush_back(pwd_lst, ft_strdup("/"));
		*arg_lst = (*arg_lst)->next;
	}
}

char	*blt_cd_restore_path(t_list *pwd_lst)
{
	char	*slash;

	slash = ft_strdup("/");
	if (slash == NULL)
	{
		ms_perror("malloc");
		return (NULL);
	}
	if (pwd_lst->next == NULL)
		return (slash);
	return (blt_slash_join_path(pwd_lst));
}

char *blt_join(char *s1, char *s2)
{
	char	*s;

	s = ft_strjoin(s1, s2);
	free(s1);
	if (s == NULL)
		return (NULL);
	return (s);
}

char *blt_slash_join_path(t_list *pwd_lst)
{
	char	*path;
	t_list	*tmp_lst;

	tmp_lst = pwd_lst->next;
	path = ft_strdup("");
	if (path == NULL)
	{
		ms_perror("malloc");
		return (NULL);
	}
	while (tmp_lst)
	{
		if (tmp_lst->content && \
			!(ft_strcmp(tmp_lst->content, "/") == 0 && tmp_lst->next))
		{
			path = blt_join(path, "/");
			if (path == NULL)
				return (NULL);
		}
		path = blt_join(path, (char *)tmp_lst->content);
		if (path == NULL)
			return (NULL);
		tmp_lst = tmp_lst->next;
	}
	ft_lstclear(&pwd_lst, free);
	return (path);
}
