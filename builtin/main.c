#include "ms_builtin.h"

static void	blt_unset_test(t_shellvar *env)
{
	printf("=====================================\n");
	blt_unset(env, "key");
	ms_exec_builtin(env, "env");
	printf("=====================================\n");
	blt_unset(env, "null");
	ms_exec_builtin(env, "env");
	printf("=====================================\n");
	blt_unset(env, "empty_string");
	ms_exec_builtin(env, "env");
	printf("=====================================\n");
	blt_unset(env, "null_update");
	ms_exec_builtin(env, "env");
	printf("=====================================\n");
	blt_unset(env, "empty_update");
	ms_exec_builtin(env, "env");
	printf("=====================================\n");
	blt_unset(env, "nosuchfile");
	ms_exec_builtin(env, "env");
}

static void blt_export_test(t_shellvar *env)
{
	blt_append_or_update_env(env, "key", "update");
	blt_append_or_update_env(env, "empty_string", "");
	blt_append_or_update_env(env, "null", NULL);
	blt_append_or_update_env(env, "null_update", "null");
	blt_append_or_update_env(env, "null_update", NULL);
	blt_append_or_update_env(env, "empty_update", "empty");
	blt_append_or_update_env(env, "empty_update", "");
	blt_print_export(env);
	printf("=====================================\n");
	blt_print_sort_env(env);
}

static void	blt_cd_pwd_oldpwd(t_shellvar *env, char *path)
{
	t_shellvar *pwd;
	t_shellvar *oldpwd;

	pwd = ms_search_key(env, "PWD");
	oldpwd = ms_search_key(env, "OLDPWD");
	printf("OLDPWD :  %s\n", oldpwd->value);
	printf("PWD    :  %s\n", pwd->value);
	printf("command: [cd %s]\n", path);
	blt_cd(env, path);
	printf("=====================================\n");

//	pwd = ms_search_key(env, "PWD");
//	oldpwd = ms_search_key(env, "OLDPWD");
//	printf("OLDPWD is %s\n", oldpwd->value);
//	printf("PWD    is %s\n", pwd->value);
}


static void blt_cd_pwd(t_shellvar *env, char *path)
{
	printf("command: [cd %s]\n", path);
	printf("before: ");
	ms_exec_builtin(env, "pwd");
	blt_cd(env, path);
	printf("after:  ");
	ms_exec_builtin(env, "pwd");
}

static void	blt_cd_test(t_shellvar *env)
{
	// printf("-----cd test-----\n");
	// blt_cd_pwd(env, "../");
	// blt_cd_pwd(env, "./");
	// blt_cd_pwd(env, "");
	// blt_cd_pwd(env, NULL);
	// blt_cd_pwd(env, "../../../../.././../");

	printf("-----cd pwd oldpwd test-----\n");
	blt_cd_pwd_oldpwd(env, "../");
	blt_cd_pwd_oldpwd(env, "./");
	blt_cd_pwd_oldpwd(env, NULL);
	blt_cd_pwd_oldpwd(env, ".");
	blt_cd_pwd_oldpwd(env, "nosuchfile");
	blt_cd_pwd_oldpwd(env, "/Users/rmatsuka/test1/test2/test3");
	blt_cd_pwd_oldpwd(env, "../../../../../../../");
	blt_cd_pwd_oldpwd(env, ".././.././.././../");
	blt_cd_pwd_oldpwd(env, "./././../../../");
}

static void	blt_exit_test(void)
{
	blt_exit("0");
	blt_exit("42");
	blt_exit("9223372036854775807");  // LONG_MAX
	blt_exit("-9223372036854775808"); // LONG_MIN
	blt_exit("9223372036854775808");  // LONG_MAX + 1
	blt_exit("-9223372036854775809"); // LONG_MIN - 1
	blt_exit("hello");
	blt_exit("42tokyo");
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

//	blt_export_test(env);
//	blt_unset_test(env);
//	blt_cd_test(env);
//	blt_exit_test();
	return (0);
}
