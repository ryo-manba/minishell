#include "ms_builtin.h"

int	ms_exec_builtin(t_shellvar *env, char *s)
{
//	if (ft_strcmp(s, "echo") == 0)
//		return (ms_echo());
	if (ft_strcmp(s, "cd") == 0)
		return (ms_cd(env, s));
	if (ft_strcmp(s, "pwd") == 0)
		return (ms_pwd());
	if (ft_strcmp(s, "export") == 0)
		return (ms_export(env, s));
	if (ft_strcmp(s, "unset") == 0)
		return (ms_unset(env, s));
	if (ft_strcmp(s, "env") == 0)
		return (ms_env(env));
//	if (ft_strcmp(s, "exit") == 0)
//		return (ms_exit());
	return (0);
}
