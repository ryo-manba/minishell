#ifndef MS_BUILTIN_H
# define MS_BUILTIN_H

#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <limits.h>
#include "../analyzer/ms_analyzer.h"

# define MS_BLT_SUCC 0
# define MS_BLT_FAIL 1

/* env */
# define KEY 0
# define VALUE 1

/* exit */
# define NOT_A_NUMBER 1
# define TOO_MANY_ARGS 2

/* ms_builtin */
int	ms_exec_builtin(t_shellvar *env, t_stree *tree);
int	ms_is_builtin(t_stree *tree);

/* blt_cd */
int			blt_cd(t_shellvar *env, t_stree *tree);
int			blt_change_directory(t_shellvar *env, t_stree *tree);
int			blt_cd_home(t_shellvar *env);
int			blt_update_pwd(t_shellvar *env, char *old_pwd);
void		blt_search_and_update_env(t_shellvar *env, char *key, char *new_value);

/* blt_echo */
int			blt_echo(t_stree *tree);
void		blt_echo_print_error(char *message);

/* blt_env */
int	ms_create_key_value(char *env, char *key_value[2]);
t_shellvar *ms_create_env(void);
int	blt_env(t_shellvar *var);

/* blt_env_utils */
void		ms_env_all_free(t_shellvar *var);
int			blt_check_malloc_key_value(t_shellvar *var, char *s, int key_or_value);
t_shellvar	*blt_new_env(char *key, char *value, int is_env);
t_shellvar	*blt_envlast(t_shellvar *env);
void		blt_env_add_back(t_shellvar **env, t_shellvar *new);

/* blt_env_sort */
void		blt_quick_sort(t_shellvar *first, t_shellvar *last);
void		blt_swap_env(t_shellvar *env1, t_shellvar *env2);
t_shellvar	*blt_partition(t_shellvar *first, t_shellvar *last);

/* blt_exit */
void		blt_exit_print_error(int flag, char *error_args);
int			blt_is_args_correct(char *args);
int			blt_check_long_overflow(char *status);
int			blt_exit(t_stree *tree);

/* blt_export */
int			blt_join_env(t_shellvar *key_pos, char *key_value[2]);
int			blt_append_or_join_env(t_shellvar *env, char* key_value[2]);
int			blt_check_and_export(t_stree *tree, t_shellvar *var, char *key_value[2]);
int	 		blt_export_env(t_shellvar *env, t_stree *tok);
int			blt_export(t_shellvar *env, t_stree *tree);

/* blt_export_check */
void		blt_export_print_error(char *message);
int			blt_check_export_key(char *token, char *key_value[2], int32_t equal_pos);
int			blt_check_and_separate_export(char *token, char *key_value[2]);

/* blt_export_print */
void		blt_print_export(t_shellvar *env);
void		blt_env_clear(t_shellvar *env);
t_shellvar	*blt_copy_env(t_shellvar *env);
void		blt_print_sort_env(t_shellvar *env);

/* blt_export_utils */
void		blt_env_free(t_shellvar *env);
t_shellvar	*blt_search_key(t_shellvar *env, char *key);
int			blt_append_env(t_shellvar *env, char *key, char *value);
int			blt_update_env(t_shellvar *update_pos, char *value);
int			blt_append_or_update_env(t_shellvar *env, char *key, char *value);

/* blt_pwd */
int			blt_pwd(void);

/* blt_unset */
int			blt_unset(t_shellvar *env, t_stree *tree);
void		blt_unset_head(t_shellvar *env);
void		blt_unset_second_and_subsequent(t_shellvar *env, t_shellvar *key_pos);

/* blt_builtin_error */
void		blt_cd_print_error(char *dirname, char *message);
#endif
