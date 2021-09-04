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
