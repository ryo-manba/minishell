#include "ms_utils.h"

//　実行パスをディレクトリから探す
char *ms_search_execution_path(DIR *dir, char *cmd, char *path)
{
	struct dirent *dirent;
	char *tmp;
	char *execution_path;

	while (1) 	// opendirに失敗したままreaddirするとセグフォする
	{
		dirent = readdir(dir);
		if (dirent == NULL)
			break ;
		if (ft_strcmp(dirent->d_name, cmd) == 0)
		{
			closedir(dir);
			tmp = ft_strjoin(path, "/");
			execution_path = ft_strjoin(tmp, cmd);
			free(tmp);
			return (execution_path);
		}
	}
	return (NULL);
}

// 実行パスを返す
// getenvはms_getenv作る
char	*ms_get_path(char *cmd)
{
	int		i;
	char	*path;
	char	**split_path;
	DIR		*dir;

	path = getenv("PATH");
	split_path = ft_split(path, ':');
	i = -1;
	while (split_path[++i])
	{
		dir = opendir(split_path[i]); // 権限がない場合もopendirに失敗する
		if (dir == NULL)
			continue ;
		path = ms_search_execution_path(dir, cmd, split_path[i]);
		if (path != NULL)
			return (path);
		closedir(dir);
	}
	return (NULL);
}
