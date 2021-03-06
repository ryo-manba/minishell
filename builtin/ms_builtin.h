/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 17:48:40 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/30 20:02:11 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_BUILTIN_H
# define MS_BUILTIN_H

# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <dirent.h>
# include <fcntl.h>
# include <sys/errno.h>
# include <limits.h>
# include "../utils/ms_utils.h"
# include "../analyzer/ms_analyzer.h"

# define MS_BLT_SUCC 0
# define MS_BLT_FAIL 1

/* env */
# define KEY 0
# define VALUE 1

/* exit */
# define NOT_A_NUMBER 1
# define TOO_MANY_ARGS 2

/* ms_builtin */
int			ms_exec_builtin(t_stree *tree, t_master *master);
int			ms_is_builtin(t_stree *tree);
int			ms_is_special_builtin(t_stree *tree);
void		ms_perror(char *func_name);
void		ms_perror_exit(char *func_name);

/* ms_create_env */
int			ms_check_malloc_key_value(
				t_shellvar *var, char *s, int key_or_value);
t_shellvar	*ms_create_append_env(char *env);
int			ms_create_env(t_master *master);
int			ms_create_key_value(char *env, char *key_value[2]);
t_shellvar	*ms_new_env(char *key, char *value, int is_env);

/* ms_env_utils */
void		ms_env_add_back(t_shellvar **env, t_shellvar *new_var);
void		ms_env_all_free(t_shellvar **var);
t_shellvar	*ms_envlast(t_shellvar *env);
void		ms_env_free(t_shellvar *env);

/* ms_env_preset */
int			ms_preset_env(t_master *master);

/* blt_cd_lst */
t_list		*blt_lst_init(t_list **del_lst);
int			blt_malloc_failed_lstclear(t_list **lst);
t_list		*blt_cd_create_list(char *s, int is_absolute);
void		blt_lstlast_del(t_list *lst);

/* blt_cd_no_current */
int			blt_cd_no_current(t_master *master, t_shellvar *env, char *arg);

/* blt_cd_normalize_path */
size_t		blt_pre_absolute_lst(t_list **lst, char *s);
char		*blt_cd_has_args(t_master *master, t_stree *tree);

/* blt_cd_normalize_utils */
char		*blt_cd_restore_path(t_list *pwd_lst);

/* blt_cd_update_pwd */
int			blt_cd_update(t_master *master, t_shellvar *env, char *pwd);
int			blt_cd_update_pwd(
				t_master *master, t_stree *tree, t_shellvar *env, int is_succ);

/* blt_cd */
int			blt_cd(t_shellvar *env, t_stree *tree, t_master *master);

/* blt_echo */
int			blt_echo(t_stree *tree, t_master *master);

/* blt_env_sort */
void		blt_quick_sort(t_shellvar *first, t_shellvar *last);

/* blt_env */
int			blt_env(t_shellvar *var);

/* blt_exit */
int			blt_exit(t_stree *tree, t_master *master);

/* blt_export_check */
int			blt_check_and_separate_env(
				t_master *master, char *token, char *key_value[2]);

/* blt_export_print */
void		blt_export_print_sorted_env(t_shellvar *env);

/* blt_export_utils */
int			blt_append_env(t_shellvar **env, char *key, char *value,
				int is_env);
int			blt_append_or_update_env(t_shellvar **env, char *key, char *value,
				int is_env);
int			blt_update_env(t_shellvar *update_pos, char *value, int is_env);
int			blt_delvalue_env(t_shellvar *update_pos);
t_shellvar	*ms_search_key(t_shellvar *env, char *key);

/* blt_export */
int			blt_export(t_stree *tree, t_master *master);

/* blt_pwd */
int			blt_pwd(t_master *master);

/* blt_unset */
int			blt_unset(t_master *master, t_stree *tree);

#endif
