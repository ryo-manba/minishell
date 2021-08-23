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


/* ms_env */
t_shellvar *ms_create_env(void);
t_shellvar	*ms_new_env(char *key, char *value, int is_env);
t_shellvar	*ms_envlast(t_shellvar *env);
void		ms_env_add_back(t_shellvar **env, t_shellvar *new);
int			ms_env(t_shellvar *env);

/* ms_export */
int	ms_export(t_shellvar *env, char *s);
int	ms_print_export(t_shellvar *env);
void	ms_env_clear(t_shellvar *env);
t_shellvar *ms_copy_env(t_shellvar *env);

/* ms_env_sort */
void	ms_quick_sort(t_shellvar *first, t_shellvar *last);
void	ms_swap_env(t_shellvar *env1, t_shellvar *env2);
t_shellvar *ms_partition(t_shellvar *first, t_shellvar *last);



#endif
