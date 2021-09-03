#include "ms_builtin.h"

int	ms_is_builtin(t_stree *tree)
{
	if (ft_strcmp(tree->token, "echo") == 0)
		return (1);
	if (ft_strcmp(tree->token, "cd") == 0)
		return (1);
	if (ft_strcmp(tree->token, "pwd") == 0)
		return (1);
	if (ft_strcmp(tree->token, "export") == 0)
		return (1);
	if (ft_strcmp(tree->token, "unset") == 0)
		return (1);
	if (ft_strcmp(tree->token, "env") == 0)
		return (1);
	if (ft_strcmp(tree->token, "exit") == 0)
		return (1);
	return (0);
}

/*
 * echo -n hello 42
 * tree->token = echo
 * tree->right->token = -n
 * tree->right->right->token = hello
 * tree->right->right->right->token = 42
 */
int	ms_exec_builtin(t_shellvar *env, t_stree *tree)
{
	if (ft_strcmp(tree->token, "echo") == 0)
		return (blt_echo(tree->right));
	if (ft_strcmp(tree->token, "cd") == 0)
		return (blt_cd(env, tree->right));
	if (ft_strcmp(tree->token, "pwd") == 0)
		return (blt_pwd());
	if (ft_strcmp(tree->token, "export") == 0)
		return (blt_export(env, tree->right));
	if (ft_strcmp(tree->token, "unset") == 0)
		return (blt_unset(env, tree->right));
	if (ft_strcmp(tree->token, "env") == 0)
		return (blt_env(env));
	if (ft_strcmp(tree->token, "exit") == 0)
		return (blt_exit(tree->right));
	return (0);
}
