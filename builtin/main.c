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

static void	ms_cd_pwd_oldpwd(t_shellvar *env, char *path)
{
	t_shellvar *pwd;
	t_shellvar *oldpwd;

	pwd = ms_search_key(env, "PWD");
	oldpwd = ms_search_key(env, "OLDPWD");
	printf("OLDPWD :  %s\n", oldpwd->value);
	printf("PWD    :  %s\n", pwd->value);
	printf("command: [cd %s]\n", path);
	ms_cd(env, path);
	printf("=====================================\n");

//	pwd = ms_search_key(env, "PWD");
//	oldpwd = ms_search_key(env, "OLDPWD");
//	printf("OLDPWD is %s\n", oldpwd->value);
//	printf("PWD    is %s\n", pwd->value);
}


static void ms_cd_pwd(t_shellvar *env, char *path)
{
	printf("command: [cd %s]\n", path);
	printf("before: ");
	ms_exec_builtin(env, "pwd");
	ms_cd(env, path);
	printf("after:  ");
	ms_exec_builtin(env, "pwd");
}

static void	ms_cd_test(t_shellvar *env)
{
	// printf("-----cd test-----\n");
	// ms_cd_pwd(env, "../");
	// ms_cd_pwd(env, "./");
	// ms_cd_pwd(env, "");
	// ms_cd_pwd(env, NULL);
	// ms_cd_pwd(env, "../../../../.././../");

	printf("-----cd pwd oldpwd test-----\n");
	ms_cd_pwd_oldpwd(env, "../");
	ms_cd_pwd_oldpwd(env, "./");
	ms_cd_pwd_oldpwd(env, NULL);
	ms_cd_pwd_oldpwd(env, ".");
	ms_cd_pwd_oldpwd(env, "nosuchfile");
	ms_cd_pwd_oldpwd(env, "/Users/rmatsuka/test1/test2/test3");
	ms_cd_pwd_oldpwd(env, "../../../../../../../");
	ms_cd_pwd_oldpwd(env, ".././.././.././../");
	ms_cd_pwd_oldpwd(env, "./././../../../");
}

static void	ms_exit_test(void)
{
	ms_exit("0");
	ms_exit("42");
	ms_exit("9223372036854775807");  // LONG_MAX
	ms_exit("-9223372036854775808"); // LONG_MIN
	ms_exit("9223372036854775808");  // LONG_MAX + 1
	ms_exit("-9223372036854775809"); // LONG_MIN - 1
	ms_exit("hello");
	ms_exit("42tokyo");
}


int main()
{
	t_shellvar *env;

	env = ms_create_env();
//	ms_exec_builtin(env, "env");
//	ms_exec_builtin(env, "export");
//	ms_exec_builtin(env, "unset");
//	ms_exec_builtin(env, "pwd");
//	ms_exec_builtin(env, "cd");
//	ms_exec_builtin(env, "exit");

//	ms_export_test(env);
//	ms_unset_test(env);
//	ms_cd_test(env);
//	ms_exit_test();
	return (0);
}
