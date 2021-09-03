#include "ms_builtin.h"

void	blt_echo_print_error(char *message)
{
	ft_putstr_fd("minishell: echo: write error: ", STDERR_FILENO);
	ft_putendl_fd(message, STDERR_FILENO);
}

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
	{
		has_op = 1;
		tree = tree->right;
	}
	while (tree != NULL)
	{
		ft_putstr_fd(tree->token, STDOUT_FILENO);
		if (tree->right != NULL)
			ft_putchar_fd(' ', STDOUT_FILENO);
		tree = tree->right;
	}
	if (has_op == 0)
		ft_putchar_fd('\n', STDOUT_FILENO);
	if (errno != 0) // なんらかで文字表示に失敗した場合(fdが存在しないとか)
	{
		blt_echo_print_error(strerror(errno));
		return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}
