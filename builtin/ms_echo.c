#include "ms_builtin.h"

/**
 * echo -n hello world
 * t_tree->token = -n
 * t_tree->right->token = hello
 * t_tree->right->right->token = world
 */
int	ms_echo(t_stree *tree)
{
	bool	has_op;

	has_op = false;
	if (ft_strcmp(tree->token, "-n") == 0)
		has_op = true;
	while (tree != NULL)
	{
		printf("%s\n", tree->token);
		tree = tree->right;
	}
	if (has_op == false)
		printf("\n");
	return (0);
}
