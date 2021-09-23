/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_get_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:59 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/24 02:03:47 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

void	exec_all_free(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

char	*exec_strjoin(char *split_path, char *cmd)
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

// err_pathは存在するが失敗した場合ようにとっておく
// ファイルまたはディレクトリが存在したらpathを返す
char	*exec_create_path(char *cmd, char **split_path)
{
	char		*path;
	char		*err_path;
	int			i;

	i = -1;
	err_path = NULL;
	while (split_path[++i])
	{
		path = exec_strjoin(split_path[i], cmd);
		if (exec_check_path(path, 1) == MS_EXEC_SUCC)
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

char	**exec_create_split_path(t_shellvar *var)
{
	t_shellvar	*path_pos;
	char		**split_path;

	path_pos = ms_search_key(var, "PATH");
	if (path_pos == NULL || path_pos->value == NULL
		|| ft_strlen(path_pos->value) == 0)
		return (NULL);
	split_path = ft_split(path_pos->value, ':');
	if (split_path == NULL)
		ms_perror_exit("malloc");
	return (split_path);
}

// PATHがunsetされてる場合は "bash: ls: No such file or directory"になる
char	*exec_get_path(char *cmd, t_shellvar *var)
{
	char	**split_path;
	char	*path;

	split_path = exec_create_split_path(var);
	if (split_path == NULL)
	{
		g_ex_states = NO_SUCH_FILE;
		return (NULL);
	}
	path = exec_create_path(cmd, split_path);
	if (!path && (g_ex_states != IS_A_DIR && g_ex_states != PERMISSION))
		g_ex_states = CMD_NOT_FOUND;
	exec_all_free(split_path);
	return (path);
}
