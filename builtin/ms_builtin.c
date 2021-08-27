#include "ms_builtin.h"

/**
 * echo -n hello 42
 * tree->token = echo
 * tree->right->token = -n
 * tree->right->right->token = hello
 * tree->right->right->right->token = 42
 */
int	ms_exec_builtin(t_shellvar *env, t_stree *tree)
{
	if (ft_strcmp(tree->token, "echo") == 0)
		return (ms_echo(tree->right));
	if (ft_strcmp(tree->token, "cd") == 0)
		return (ms_cd(env, tree->token));
	if (ft_strcmp(tree->token, "pwd") == 0)
		return (ms_pwd());
	if (ft_strcmp(tree->token, "export") == 0)
		return (ms_export(env, tree->right));
	if (ft_strcmp(tree->token, "unset") == 0)
		return (ms_unset(env, tree->right));
	if (ft_strcmp(tree->token, "env") == 0)
		return (ms_env(env));
	if (ft_strcmp(tree->token, "exit") == 0)
		return (ms_exit(tree->right));
	return (0);
}
