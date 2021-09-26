/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:09:16 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/26 16:48:11 by yokawada         ###   ########.fr       */
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
int		exec_expand_redir_loop(t_master *master,
			t_redir *rd, t_ex_state *es, int stdin_copy);
int		exec_expand_redirect(t_master *master,
			t_clause *clause);

/* exec_backup_fd */
int		exec_close_backup_fd(int backup_fd[3]);
int		exec_create_backup_fd(int backup_fd[3]);
int		exec_duplicate_backup_fd(int backup_fd[3]);

/* exec_check_path_stat */
int		exec_check_path_stat(char *path, int is_command);
int		exec_check_cmd_stat(char *path);

/* exec_get_path */
void	exec_all_free(char **s);
char	*exec_create_path(char *cmd, char **split_path, int is_command);
char	**exec_create_split_path(t_shellvar *var, int *look_from_cd);
char	*exec_get_path(char *cmd, t_shellvar *var);
char	*exec_strjoin(char *path, char *cmd);

/* exec_main_flow */
char	**exec_create_command(t_stree *tree);
void	exec_update_exitstatus(pid_t pid);
int		ms_executer(t_pipeline *pl, t_master *master, t_ex_state *state);
void	ms_close_heredoc_fd(t_clause *cl);

/* exec_pipe_cmd */
int		exec_check_piping(t_dpipe *dpipe, t_clause *clause);
void	exec_pipe_child(t_clause *clause, t_master *master,
			t_ex_state *state, t_dpipe *dpipe);
pid_t	exec_pipe_cmd_loop(
			t_master *master, t_clause *cl, t_dpipe *dpipe, t_ex_state *state);
int		exec_pipe_command(t_pipeline *pl, t_master *master, t_ex_state *state);
void	exec_pipe_parent(t_dpipe *dpipe);

/* exec_restore_cmd */
int		exec_get_env_size(t_shellvar *var);
char	*exec_restore(t_shellvar *var);
char	**exec_restore_env(t_shellvar *var);

/* exec_run */
void	exec_run_cmd_exit(t_master *master, t_stree *expanded, t_shellvar *var);

/* exec_simple_cmd */
int		exec_ex_cmd(t_master *master, t_shellvar *var, t_stree *expanded);
int		exec_simple_command(
			t_clause *clause, t_master *master, t_ex_state *state);
int		exec_simple_redir(t_master *master,
			t_clause *clause, int backup_fd[3]);
int		exec_subshell(t_clause *clause, t_master *master, t_ex_state *es);

/* exec_utils */
void	exec_check_path_exit(t_master *master, t_stree *expanded, char *path);
size_t	exec_get_command_size(t_stree *tree);
void	exec_set_signal_wait(pid_t pid);
int		exec_out(int rv, t_stree *expanded);

/* ms_pipe */
void	ms_close_and_update_pipe(int pipe_fd[2], int before_pipe[2]);
void	ms_do_piping(t_clause *test, int pipe_fd[2], int before_pipe[2]);
void	ms_first_pipe(int pipe_fd[2]);
void	ms_last_pipe(int before_pipe[2]);
void	ms_middle_pipe(int pipe_fd[2], int before_pipe[2]);

/* ms_redir_error */
int		ms_check_fd(char *fd);
int		ms_check_fd_print_error(t_redir *rd, t_master *master);
void	ms_redir_print_error(t_master *master, int err, char *s);

/* ms_redirect */
int		ms_open_at(int fd, const char *path, int oflag, int mode);
int		ms_open_redirect_append(t_redir *redir);
int		ms_open_redirect_input(t_redir *redir);
int		ms_open_redirect_output(t_redir *redir);
int		ms_redirect(t_ex_state *es, t_redir *redir);

/* ms_redir_heredoc_io */
int		ms_heredoc_read(t_list **lst, char *delimiter);
int		ms_heredoc_write(t_ex_state *es, t_list *lst, int quoted, int fd);
int		ms_heredoc_read_write(t_ex_state *es, t_redir **redir, int write_fd);

/* ms_redir_heredoc */
int		ms_heredoc_child(t_redir **rd, t_ex_state *es, int pipefd[2]);
int		ms_heredoc_parent(t_redir **rd, int pipefd[2]);
int		ms_heredoc_rd_loop(t_redir **rd, t_ex_state *es);
int		ms_heredoc_pipe_loop(t_clause **cl, t_ex_state *es);
int		ms_heredoc(t_clause **cl, t_ex_state *es);

/* ms_signal_handler */
void	ms_heredoc_sigint_handler(int sig);
int		ms_heredoc_signal_set(void);

#endif
