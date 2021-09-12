/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_get_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:59 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/12 22:30:51 by rmatsuka         ###   ########.fr       */
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

// 実行可能ビットが立ってたらOK (S_IXUSR (00100))所有者による実行
int	exec_check_path(struct stat sb)
{
	if ((sb.st_mode & S_IXUSR))
	{
		g_ex_states = 0;
		return (MS_EXEC_SUCC);
	}
	g_ex_states = PERMISSION;
	if ((sb.st_mode & S_IFMT) == S_IFREG)
		g_ex_states = IS_A_DIR;
	return (MS_EXEC_FAIL);
}

// err_pathは存在するが失敗した場合ようにとっておく
// ファイルまたはディレクトリが存在したらpathを返す
char	*exec_create_path(char *cmd, char **split_path)
{
	char		*path;
	char		*err_path;
	char		*tmp;
	struct stat	sb;
	int			i;

	i = -1;
	err_path = NULL;
	while (split_path[++i])
	{
		tmp = ft_strjoin(split_path[i], "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (stat(path, &sb) >= 0 && exec_check_path(sb) == 0)
		{
			free(err_path);
			return (path);
		}
		tmp = err_path;
		err_path = path;
		free(tmp);
	}
	return (err_path);
}

char	**exec_create_split_path(t_shellvar *var)
{
	t_shellvar	*path_pos;
	char		**split_path;

	path_pos = ms_search_key(var, "PATH");
	if (path_pos == NULL)
		return (NULL);
	split_path = ft_split(path_pos->value, ':');
	if (split_path == NULL)
		return (NULL);
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
	if (path == NULL && \
		(g_ex_states != IS_A_DIR || g_ex_states != PERMISSION))
		g_ex_states = CMD_NOT_FOUND;
	exec_all_free(split_path);
	return (path);
}
