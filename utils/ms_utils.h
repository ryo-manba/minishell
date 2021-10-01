/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:09:16 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/10/01 09:35:57 by rmatsuka         ###   ########.fr       */
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
# include "../minishell.h"
# include <readline/readline.h>
# include <readline/history.h>

# define MS_EXEC_SUCC 0
# define MS_EXEC_FAIL 1

/* ms_get_execution_path */
# define IS_A_DIR 120
# define PERMISSION 121
# define CMD_NOT_FOUND 122
# define NO_SUCH_FILE 123
# define NOT_A_DIR 126
# define ERRNO_NO_SUCH_FILE 2
# define ERRNO_EXECVE_FORMAT_ERROR 8
# define ERRNO_NOT_A_DIR 20

/* ms_execute_utils */
# define OVER_FD -1
# define OVER_INT -2

typedef struct s_dpipe
{
	int	new[2];
	int	before[2];
}	t_dpipe;

extern volatile sig_atomic_t	g_ex_states;

/* exec_error */
void	exec_error_prologue(t_master *master, int limit_line);
void	exec_print_error_exit(t_master *master, int ex_status, char *path);
void	exec_print_prologue(t_master *master);

/* exec_expand_redir */
int		exec_expand_redirect(
			t_master *master, t_clause *clause, int backup_fd[3]);

/* exec_backup_fd */
int		exec_create_backup_fd(int backup_fd[3]);
int		exec_duplicate_backup_fd(int backup_fd[3]);

/* exec_check_path_stat */
int		exec_check_path_stat(char *path, int is_command);

/* exec_get_path */
void	exec_all_free(char **s);
char	*exec_get_path(char *cmd, t_shellvar *var);

/* exec_main_flow */
void	exec_update_exitstatus(pid_t pid);
int		ms_executer_wrapper(
			t_pipeline *pl, t_master *master, t_ex_state *state);
int		ms_executer(t_pipeline *pl, t_master *master, t_ex_state *state);
void	ms_close_heredoc_fd(t_pipeline *pl);

/* exec_pipe_cmd */
int		exec_pipe_command(t_pipeline *pl, t_master *master, t_ex_state *state);

/* exec_restore_env */
char	**exec_restore_env(t_shellvar *var);

/* exec_run */
void	exec_run_cmd_exit(t_master *master, t_stree *expanded, t_shellvar *var);

/* exec_update_exitstatus */
void	exec_update_exitstatus(pid_t pid);

/* exec_simple_cmd */
int		exec_subshell(t_clause *clause, t_master *master,
			t_ex_state *es, int backup_fd[3]);
int		exec_simple_command(
			t_clause *clause, t_master *master, t_ex_state *state);

/* exec_utils */
void	exec_set_signal_wait(pid_t pid);
int		exec_out(int rv, t_stree *expanded);

/* ms_pipe */
void	ms_close_and_update_pipe(int pipe_fd[2], int before_pipe[2]);
void	ms_do_piping(t_clause *test, int pipe_fd[2], int before_pipe[2]);

/* ms_redir_error */
int		ms_check_fd_print_error(t_redir *rd, t_master *master);

/* ms_redir_heredoc_io */
int		ms_heredoc_read_write(t_ex_state *es, t_redir **redir, int write_fd);
int		ms_redir_is_heredoc(t_redir *redir);

/* ms_redir_heredoc */
int		ms_heredoc(t_pipeline *pl, t_ex_state *es);

/* ms_redir */
int		ms_redirect(t_ex_state *es, t_redir *redir);
int		ms_open_at(int fd, const char *path, int oflag, int mode);

#endif
