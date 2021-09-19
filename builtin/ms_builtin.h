/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 17:48:40 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/19 11:32:40 by yokawada         ###   ########.fr       */
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
void		ms_perror(char *func_name);
void		ms_perror_exit(char *func_name);

/* ms_create_env */
int			ms_check_malloc_key_value(
				t_shellvar *var, char *s, int key_or_value);
t_shellvar	*ms_create_append_env(char *env);
int			ms_create_env(t_shellvar **var);
int			ms_create_key_value(char *env, char *key_value[2]);
t_shellvar	*ms_new_env(char *key, char *value);

/* ms_env_utils */
void		ms_env_add_back(t_shellvar **env, t_shellvar *new_var);
void		ms_env_all_free(t_shellvar **var);
t_shellvar	*ms_envlast(t_shellvar *env);
void		ms_env_free(t_shellvar *env);

/* ms_env_preset */
int			ms_preset_env(t_shellvar **var);

/* blt_cd_no_current */
char		*blt_check_slash_join(char *env_pwd, char *arg);
int			blt_cd_no_current(t_shellvar *env, char *arg);
int			blt_cd_no_prevdir(t_shellvar *env, char *now_pwd);

/* blt_cd_update_pwd */
int			blt_cd_update(t_shellvar *env, char *old_pwd, char *now_pwd);
int			blt_cd_update_pwd(t_shellvar *env, t_stree *tree, char *old_pwd);

/* blt_cd */
int			blt_cd(t_shellvar *env, t_stree *tree, t_master *master);
int			blt_cd_change_dir(t_shellvar *env, t_stree *tree, t_master *master);
int			blt_cd_home(t_shellvar *env, t_master *master);
void		blt_cd_print_error(t_master *master, char *dirname, char *message);

/* blt_echo */
int			blt_echo(t_stree *tree, t_master *master);
void		blt_echo_print_error(t_master *master, char *message);

/* blt_env_sort */
t_shellvar	*blt_partition(t_shellvar *first, t_shellvar *last);
void		blt_quick_sort(t_shellvar *first, t_shellvar *last);
void		blt_swap_env(t_shellvar *env1, t_shellvar *env2);

/* blt_env */
int			blt_search_and_update_env(
				t_shellvar *env, char *key, char *new_value);
int			blt_env(t_shellvar *var);

/* blt_exit */
int			blt_check_long_overflow(char *status);
int			blt_exit(t_stree *tree, t_master *master);
void		blt_exit_print_error(t_master *master, int flag, char *error_args);
int			blt_is_args_correct(char *args);

/* blt_export_check */
int			blt_check_and_separate_env(
				t_master *master, char *token, char *key_value[2]);
int			blt_check_export_key(char *token, int32_t equal_pos);
void		blt_export_print_error(t_master *master, char *message);
int			blt_separate_key_value(
				int32_t equal_idx, char *token, char *key_value[2]);

/* blt_export_print */
int			blt_check_escape(char *s);
t_shellvar	*blt_copy_env(t_shellvar *env);
void		blt_print_export(t_shellvar *env);
void		blt_print_sort_env(t_shellvar *env);

/* blt_export_utils */
int			blt_append_env(t_shellvar *env, char *key, char *value);
int			blt_append_or_update_env(t_shellvar *env, char *key, char *value);
int			blt_update_env(t_shellvar *update_pos, char *value);
t_shellvar	*ms_search_key(t_shellvar *env, char *key);

/* blt_export */
int			blt_append_or_join_env(t_shellvar *env, char *key_value[2]);
int			blt_check_and_export(
				t_stree *tree, t_shellvar *var, char *key_value[2]);
int			blt_export(t_shellvar *env, t_stree *tree, t_master *master);
int			blt_export_env(t_shellvar *env, t_stree *tree, t_master *master);
int			blt_join_env(t_shellvar *key_pos, char *key_value[2]);

/* blt_pwd */
int			blt_print_env_pwd(t_shellvar *var);
int			blt_pwd(t_shellvar *var);

/* blt_unset */
int			blt_unset(t_master *master, t_stree *tree);
void		blt_unset_head(t_shellvar **env);
void		blt_unset_second_and_subsequent(
				t_shellvar *env, t_shellvar *key_pos);

#endif
