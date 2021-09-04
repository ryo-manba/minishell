#include "ms_utils.h"
#include <sys/stat.h>

void	ms_all_free(char **s)
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

char *ms_create_split_path(t_shellvar *var, char **split_path)
{
	t_shellvar	*path_pos;

	path_pos = blt_search_key(var, "PATH");
	if (path_pos == NULL)
		return (NULL);
	split_path = ft_split(path_pos->value, ':');
	if (split_path == NULL)
		return (NULL);
	return (split_path);
}

char	*ms_check_and_create_path(char *cmd, char **split_path, t_ex_state *state)
{
	char		*path;
	char		*err_path; // 存在するが失敗した場合ようにとっておく
	struct stat	sb;
	int			i;

	i = -1;
	err_path = NULL;
	while (split_path[++i])
	{
		path = ft_strjoin(split_path[i], cmd);
		if (stat(path, &sb) >= 0) //ファイルまたはディレクトリが存在する
		{
			state->last_exit_status = PERMISSION;
			if ((sb.st_mode & S_IFMT) == S_IFREG) // これだったらディレクトリ
				state->last_exit_status = IS_A_DIR;
			else if (sb.st_mode & S_IXUSR == 1) // 実行可能ビットが立ってたらOK (S_IXUSR (00100))所有者による実行
			{
				state->last_exit_status = 0;
				return (path);
			}
			if (err_path != NULL)
				free(err_path);
			err_path = path;
		}
		else
			free(path);
	}
	return (err_path);
}

char	*ms_get_path(char *cmd, t_shellvar *var, t_ex_state *state)
{
	int	i;
	char **split_path;
	char *path;
	struct stat sb;

	if (ms_create_split_path(var, split_path) == NULL)
		return (NULL);
	path = ms_check_and_create_path(cmd, split_path, state);
	if (path == NULL && (state->last_exit_status != IS_A_DIR
		|| state->last_exit_status != PERMISSION))
		state->last_exit_status = CMD_NOT_FOUND;
	ms_all_free(split_path);
	return (path);
}
