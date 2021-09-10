/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:09:16 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/10 23:26:57 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_UTILS_H
# define MS_UTILS_H

# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <dirent.h>
# include <fcntl.h>
# include <sys/errno.h>
# include <sys/stat.h>
# include "../libft/libft.h"
# include "../analyzer/ms_analyzer.h"
# include <readline/readline.h>
# include <readline/history.h>

# define MS_EXEC_SUCC 0
# define MS_EXEC_FAIL 1

/* ms_get_execution_path */
# define IS_A_DIR 1260
# define PERMISSION 1261
# define	CMD_NOT_FOUND 127
# define NO_SUCH_FILE 128

/* ms_execute_utils */
# define OVER_FD -1
# define OVER_INT -2

typedef struct s_dpipe
{
	int	new[2];
	int	before[2];
}	t_dpipe;

/* exec_error */
void	exec_print_error(char *command);
void	exec_print_error_exit(int ex_status, char *path);

/* exec_get_path */
void	exec_all_free(char **s);
int		exec_check_path(struct stat sb, t_ex_state *state);
char	*exec_create_path(char *cmd, char **split_path, t_ex_state *state);
char	**exec_create_split_path(t_shellvar *var);
char	*exec_get_path(char *cmd, t_shellvar *var, t_ex_state *state);

/* exec_just_open */
void	exec_all_open(t_redir *expand_rd);
void	exec_just_open(t_clause *clause, t_shellvar *var);

/* exec_main_flow */
char	**exec_create_command(t_stree *tree);
int		exec_expand_redirect(t_clause *clause, t_shellvar *var);
void	exec_update_exitstatus(t_ex_state *state, pid_t pid);
int		ms_executer(t_pipeline *pl, t_shellvar *var, t_ex_state *state);

/* exec_pipe_cmd */
int		exec_check_piping(t_dpipe *dpipe, t_clause *clause);
void	exec_pipe_child(
			t_pipeline *pl, t_shellvar *var, t_ex_state *state, t_dpipe *dpipe);
int		exec_pipe_command(t_pipeline *pl, t_shellvar *var, t_ex_state *state);
void	exec_pipe_parent(t_dpipe *dpipe);
void	exec_run_cmd_exit(
			t_stree *expanded, t_shellvar *var, t_ex_state *state);

/* exec_simple_cmd */
int		exec_check_path_state(t_ex_state *es, t_stree *expanded, char *path);
int		exec_child(t_shellvar *var, t_stree *expanded);
int		exec_create_backup_fd(int backup_fd[3]);
int		exec_duplicate_backup_fd(int backup_fd[3]);
int		exec_simple_command(
			t_clause *clause, t_shellvar *var, t_ex_state *state);

/* exec_utils */
size_t	exec_get_command_size(t_stree *tree);

/* ms_pipe */
void	ms_close_and_update_pipe(int pipe_fd[2], int before_pipe[2]);
void	ms_do_piping(t_clause *test, int pipe_fd[2], int before_pipe[2]);
void	ms_first_pipe(int pipe_fd[2]);
void	ms_last_pipe(int before_pipe[2]);
void	ms_middle_pipe(int pipe_fd[2], int before_pipe[2]);

/* ms_redir_error */
int		ms_check_fd(char *fd);
int		ms_check_fd_print_error(t_redir *rd);
void	ms_redir_print_error(int err, char *s);

/* ms_redirect */
int		ms_open_at(int fd, const char *path, int oflag, int mode);
int		ms_open_redirect_append(t_redir *redir);
int		ms_open_redirect_input(t_redir *redir);
int		ms_open_redirect_output(t_redir *redir);
int		ms_redirect(t_redir *redir, t_shellvar *var);

/* ms_redir_heredoc */
int		ms_heredoc_read(t_list **lst, char *delimiter);
void	ms_heredoc_sigint_handler(int sig);
int		ms_heredoc_signal_set(void);
int		ms_heredoc_write(t_list *lst, t_shellvar *var, int quoted, int fd);
int		ms_redirect_heredoc(t_redir *redir, t_shellvar *var);

#endif
