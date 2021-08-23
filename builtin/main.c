#include "ms_builtin.h"

static void	ms_unset_test(t_shellvar *env)
{
	printf("=====================================\n");
	ms_unset(env, "key");
	ms_exec_builtin(env, "env");
	printf("=====================================\n");
	ms_unset(env, "null");
	ms_exec_builtin(env, "env");
	printf("=====================================\n");
	ms_unset(env, "empty_string");
	ms_exec_builtin(env, "env");
	printf("=====================================\n");
	ms_unset(env, "null_update");
	ms_exec_builtin(env, "env");
	printf("=====================================\n");
	ms_unset(env, "empty_update");
	ms_exec_builtin(env, "env");
	printf("=====================================\n");
	ms_unset(env, "nosuchfile");
	ms_exec_builtin(env, "env");
}

static void ms_export_test(t_shellvar *env)
{
	ms_append_or_update_env(env, "key", "update");
	ms_append_or_update_env(env, "empty_string", "");
	ms_append_or_update_env(env, "null", NULL);
	ms_append_or_update_env(env, "null_update", "null");
	ms_append_or_update_env(env, "null_update", NULL);
	ms_append_or_update_env(env, "empty_update", "empty");
	ms_append_or_update_env(env, "empty_update", "");
	ms_print_export(env);
	printf("=====================================\n");
	ms_print_sort_env(env);
}

int main()
{
	t_shellvar *env;

	env = ms_create_env();
//	ms_exec_builtin(env, "env");
//	ms_exec_builtin(env, "export");
//	ms_exec_builtin(env, "unset");
	ms_export_test(env);
	ms_unset_test(env);

	return (0);
}
