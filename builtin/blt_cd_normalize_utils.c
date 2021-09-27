/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd_normalize_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 12:31:03 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/27 10:32:31 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

static char	*blt_join(char *s1, char *s2)
{
	char	*s;

	s = ft_strjoin(s1, s2);
	free(s1);
	if (s == NULL)
		return (NULL);
	return (s);
}

static char	*blt_slash_join_loop(t_list *pwd_lst)
{
	char	*path;

	path = ft_strdup("");
	if (path == NULL)
		return (NULL);
	while (pwd_lst)
	{
		if (pwd_lst->content && \
			!(ft_strcmp(pwd_lst->content, "/") == 0 && pwd_lst->next))
		{
			path = blt_join(path, "/");
			if (path == NULL)
				return (NULL);
		}
		path = blt_join(path, (char *)pwd_lst->content);
		if (path == NULL)
			return (NULL);
		pwd_lst = pwd_lst->next;
	}
	return (path);
}

static char	*blt_slash_join_path(t_list *pwd_lst)
{
	t_list	*tmp_lst;
	char	*path;

	tmp_lst = pwd_lst->next;
	path = blt_slash_join_loop(tmp_lst);
	if (path == NULL)
	{
		blt_malloc_failed_lstclear(&pwd_lst);
		return (NULL);
	}
	ft_lstclear(&pwd_lst, free);
	return (path);
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
	free(slash);
	return (blt_slash_join_path(pwd_lst));
}
