/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_get_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:59 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/30 20:20:18 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

void	exec_all_free(char **s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

static char	*exec_strjoin(char *split_path, char *cmd)
{
	char	*tmp;
	char	*path;

	tmp = ft_strjoin(split_path, "/");
	if (!tmp)
		ms_perror_exit("malloc");
	path = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!split_path)
		ms_perror_exit("malloc");
	return (path);
}

// err_path is a path that exists but is kept in case of failure.
static char	*exec_create_path(
		char *cmd, char **split_path, int is_command)
{
	char		*dir;
	char		*path;
	char		*err_path;
	ssize_t		i;

	i = -1;
	err_path = NULL;
	while (split_path[++i])
	{
		dir = split_path[i];
		if (ft_strlen(dir) == 0)
			dir = ".";
		path = exec_strjoin(dir, cmd);
		if (exec_check_path_stat(path, is_command) == MS_EXEC_SUCC)
		{
			free(err_path);
			return (path);
		}
		if (err_path == NULL && g_ex_states == PERMISSION)
			err_path = path;
		else
			free(path);
	}
	return (err_path);
}

static char	**exec_create_split_path(t_shellvar *var, int *look_from_cd)
{
	t_shellvar	*path_pos;
	char		**split_path;

	*look_from_cd = 0;
	path_pos = ms_search_key(var, "PATH");
	if (path_pos == NULL || path_pos->value == NULL
		|| ft_strlen(path_pos->value) == 0)
	{
		*look_from_cd = 1;
		split_path = ft_split_rough(".", ':');
	}
	else
		split_path = ft_split_rough(path_pos->value, ':');
	if (split_path == NULL)
		ms_perror_exit("malloc");
	return (split_path);
}

char	*exec_get_path(char *cmd, t_shellvar *var)
{
	char	**split_path;
	char	*path;
	int		look_from_cd;

	split_path = exec_create_split_path(var, &look_from_cd);
	if (split_path == NULL)
	{
		g_ex_states = NO_SUCH_FILE;
		return (NULL);
	}
	path = exec_create_path(cmd, split_path, !look_from_cd);
	exec_all_free(split_path);
	return (path);
}
