#ifndef MS_UTILS_H
# define MS_UTILS_H

#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
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
int	ms_exec_builtin(t_shellvar *env, char *s);

/* ms_env */
t_shellvar	*ms_create_env(void);
t_shellvar	*ms_new_env(char *key, char *value, int is_env);
t_shellvar	*ms_envlast(t_shellvar *env);
void		ms_env_add_back(t_shellvar **env, t_shellvar *new);
int			ms_env(t_shellvar *env);

/* ms_env_sort */
void		ms_quick_sort(t_shellvar *first, t_shellvar *last);
void		ms_swap_env(t_shellvar *env1, t_shellvar *env2);
t_shellvar *ms_partition(t_shellvar *first, t_shellvar *last);

/* ms_export */
void		ms_join_env(t_shellvar *key_pos, char *key_value[2]);
void		ms_append_or_join_env(t_shellvar *env, char* key_value[2]);
void	 	ms_export_env(t_shellvar *env, t_token *tok);
int			ms_export(t_shellvar *env, char *s);

/* ms_export_check */
bool	ms_check_export_key(char *s, char *key_value[2], int32_t equal_pos);
bool	ms_check_and_separate_export(char *s, char *key_value[2]);

/* ms_export_print */
int			ms_print_export(t_shellvar *env);
void		ms_env_clear(t_shellvar *env);
t_shellvar	*ms_copy_env(t_shellvar *env);
void		ms_print_sort_env(t_shellvar *env);

/* ms_export_utils */
t_shellvar	*ms_search_key(t_shellvar *env, char *key);
void		ms_env_free(t_shellvar *env);
void		ms_append_env(t_shellvar *env, char *key, char *value);
void		ms_update_env(t_shellvar *update_pos, char *value);
int			ms_append_or_update_env(t_shellvar *env, char *key, char *value);

/* ms_unset */
int		ms_unset(t_shellvar *env, char *key);
void	ms_unset_head(t_shellvar *env);
void	ms_unset_second_and_subsequent(t_shellvar *env, t_shellvar *key_pos);

/* ms_pwd */
int	ms_pwd(void);

/* ms_cd */
int	ms_cd(t_shellvar *env, char *arg);
int	ms_cd_home(t_shellvar *env);

/* ms_exit */
int	ms_exit(char *arg);

/* ms_echo */

#endif
