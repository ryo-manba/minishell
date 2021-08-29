#include "ms_builtin.h"

/**
 * $ echo -n hello world
 * tree->token = -n
 * tree->right->token = hello
 * tree->right->right->token = world
 */
int	blt_echo(t_stree *tree)
{
	int	has_op;

	errno = 0;
	has_op = 0;
	if (tree != NULL && ft_strcmp(tree->token, "-n") == 0) // treeがNULLの場合は引数なし、改行だけ出力する
		has_op = 1;
	while (tree != NULL)
	{
		printf("%s", tree->token);
		if (tree->right != NULL)
			printf(" ");
		tree = tree->right;
	}
	if (has_op == 0)
		printf("\n");
	if (errno != 0) // なんらかで文字表示に失敗した場合
		return (1);
	return (0);
}
