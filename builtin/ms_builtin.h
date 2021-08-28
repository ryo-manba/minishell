#ifndef MS_UTILS_H
# define MS_UTILS_H

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
#include "../libft/libft.h"
#include "../lexer/ms_lexer.h"
#include "../analyzer/ms_analyzer.h"

typedef struct s_shellvar
{
	char	*key;
	char	*value;
	int		is_env;
	int		attr;
	struct s_shellvar *next;
}	t_shellvar;

/* ms_builtin */
int	ms_exec_builtin(t_shellvar *env, t_stree *tree);

/* ms_cd */
int			ms_cd(t_shellvar *env, t_stree *tree);
int			ms_change_directory(t_shellvar *env, t_stree *tree);
int			ms_cd_home(t_shellvar *env);
int			ms_update_pwd(t_shellvar *env, char *old_pwd);
void		ms_search_and_update_env(t_shellvar *env, char *key, char *new_value);

/* ms_echo */
int			ms_echo(t_stree *tree);

/* ms_env */
t_shellvar	*ms_create_env(void);
t_shellvar	*ms_new_env(char *key, char *value, int is_env);
t_shellvar	*ms_envlast(t_shellvar *env);
void		ms_env_add_back(t_shellvar **env, t_shellvar *new);
int			ms_env(t_shellvar *env);

/* ms_env_sort */
void		ms_quick_sort(t_shellvar *first, t_shellvar *last);
void		ms_swap_env(t_shellvar *env1, t_shellvar *env2);
t_shellvar	*ms_partition(t_shellvar *first, t_shellvar *last);

/* ms_exit */
void		ms_exit_argument_error(int flag, char *error_args);
int			ms_is_args_digit(char *args);
int			ms_check_long_overflow(char *status);
int			ms_exit(t_stree *tree);

/* ms_export */
void		ms_join_env(t_shellvar *key_pos, char *key_value[2]);
void		ms_append_or_join_env(t_shellvar *env, char* key_value[2]);
void	 	ms_export_env(t_shellvar *env, t_stree *tok);
int			ms_export(t_shellvar *env, t_stree *tree);

/* ms_export_check */
int			ms_check_export_key(char *token, char *key_value[2], int32_t equal_pos);
int			ms_check_and_separate_export(char *token, char *key_value[2]);

/* ms_export_print */
void		ms_print_export(t_shellvar *env);
void		ms_env_clear(t_shellvar *env);
t_shellvar	*ms_copy_env(t_shellvar *env);
void		ms_print_sort_env(t_shellvar *env);

/* ms_export_utils */
t_shellvar	*ms_search_key(t_shellvar *env, char *key);
void		ms_env_free(t_shellvar *env);
void		ms_append_env(t_shellvar *env, char *key, char *value);
void		ms_update_env(t_shellvar *update_pos, char *value);
int			ms_append_or_update_env(t_shellvar *env, char *key, char *value);

/* ms_pwd */
int			ms_pwd(void);

/* ms_unset */
int			ms_unset(t_shellvar *env, t_stree *tree);
void		ms_unset_head(t_shellvar *env);
void		ms_unset_second_and_subsequent(t_shellvar *env, t_shellvar *key_pos);

#endif
