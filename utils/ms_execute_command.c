#include "ms_utils.h"

// コマンドの引数を数える
// $ "ls -l" = 2
size_t	ms_get_cmd_size(t_stree *tree)
{
	t_stree *tmp;
	size_t	sz;

	tmp = tree;
	sz = 0;
	while (tmp != NULL)
	{
		tmp = tmp->right;
		sz++;
	}
	return (sz);
}

// execveに送る場合
// {"ls", "-l", "NULL"}の状態にする。
char	**ms_create_execute_command(t_stree *tree)
{
	char **new_cmd;
	size_t	sz;
	size_t	i;
	t_stree	*tmp;

	sz = ms_get_cmd_size(tree);
	new_cmd = (char **)malloc(sizeof(char *) * sz + 1);
	if (new_cmd == NULL)
		return (NULL);
	tmp = tree;
	i = 0;
	while (tmp != NULL)
	{
		new_cmd[i] = ft_strdup(tmp->token);
		tmp = tmp->right;
		i++;
	}
	new_cmd[i] = NULL;
	return (new_cmd);
}
