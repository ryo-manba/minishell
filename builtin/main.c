#include "ms_builtin.h"


int main()
{
	t_shellvar *env;

	env = ms_create_env();
//	ms_exec_builtin(env, "env");
	ms_exec_builtin(env, "export");

	return (0);
}
