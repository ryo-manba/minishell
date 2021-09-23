/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd_normalize_path.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 12:29:25 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/23 20:48:25 by rmatsuka         ###   ########.fr       */
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

int	blt_pre_absolute_lst(t_list **lst, char *s)
{
	size_t	i;

	i = 0;
	if ((ft_strlen(s) == 1 && ft_strcmp(s, "/") == 0))
		return (0);
	if (ft_strlen(s) == 2 && ft_strcmp(s, "//") == 0)
	{
		ft_lstpush_back(lst, ft_strdup(s));
		return (0);
	}
	if (ft_strlen(s) >= 2 && ft_strncmp(s, "//", 2) == 0 && s[2] != '/')
		ft_lstpush_back(lst, ft_strdup("/"));
	while (s[i] == '/')
		i += 1;
	if (i == ft_strlen(s))
		return (0);
	return (i);
}

char	*blt_cd_create_path(t_list *pwd_lst, t_list *arg_lst, int is_absolute)
{
	t_list	*tmp_lst;

	tmp_lst = arg_lst->next;
	if (is_absolute)
		blt_pre_absolute_path(&pwd_lst, &tmp_lst);
	while (tmp_lst)
	{
		if (ft_strcmp(tmp_lst->content, ".") == 0)
			;
		else if (ft_strlen(tmp_lst->content) == 2
			&& ft_strcmp(tmp_lst->content, "..") == 0)
			blt_lstlast_del(pwd_lst);
		else
			ft_lstpush_back(&pwd_lst, ft_strdup(tmp_lst->content));
		tmp_lst = tmp_lst->next;
	}
	ft_lstclear(&arg_lst, free);
	return (blt_cd_restore_path(pwd_lst));
}

char	*blt_cd_has_args(t_master *master, t_stree *tree)
{
	t_list	*pwd_lst;
	t_list	*arg_lst;
	int		is_absolute;

	is_absolute = 0;
	pwd_lst = blt_cd_create_list(master->pwd, 1);
	if (pwd_lst == NULL)
		return (NULL);
	if (tree->token[0] == '/')
		is_absolute = 1;
	arg_lst = blt_cd_create_list(tree->token, is_absolute);
	if (arg_lst == NULL)
	{
		ft_lstclear(pwd_lst, free);
		return (NULL);
	}
	return (blt_cd_create_path(pwd_lst, arg_lst, is_absolute));
}
