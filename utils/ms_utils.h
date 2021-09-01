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
#include "../libft/libft.h"
#include "../lexer/ms_lexer.h"
#include "../analyzer/ms_analyzer.h"

typedef struct	s_dpipe
{
	int	new[2];
	int	before[2];
}	t_dpipe;

/* ms_pipe.c */
int 	ms_first_pipe(int pipe_fd[2]);
int 	ms_last_pipe(int before_pipe[2]);
int 	ms_middle_pipe(int pipe_fd[2], int before_pipe[2]);
void 	ms_close_and_update_pipe(int pipe_fd[2], int before_pipe[2]);
int		ms_do_piping(t_clause *test, int pipe_fd[2], int before_pipe[2]);

/* ms_executer */
void	ms_expand_and_redirect(t_clause *clause);
int		ms_execute_pipe_parent(t_pipeline *pl, t_ex_state *state, t_dpipe *dpipe ,pid_t pid);
int		ms_execute_pipe_child(t_pipeline *pl, t_shellvar *var, t_ex_state *state, t_dpipe *dpipe);
void	ms_wait_child(int sz);
void	ms_update_exitstatus(t_ex_state *state, pid_t pid);
int		ms_execute_pipe_command(t_pipeline *pl, t_shellvar *var, t_ex_state *state);
int		ms_executer(t_pipeline *pl, t_shellvar *var, t_ex_state *state);


/* ms_execute_command */
void	ms_execute_command(t_clause *test, char **envp);

/* ms_execute_simple_command */
void	ms_print_exec_error(t_clause *clause);
int		ms_duplicate_backup_fd(int backup_fd[3]);
int		ms_create_backup_fd(int backup_fd[3]);
int		ms_execute_child(t_clause *clause);
int		ms_simple_command(t_clause *clause, t_shellvar *var);



/* ms_get_execution_path */
char *ms_search_execution_path(DIR *dir, char *cmd, char *path);
char *ms_get_path(char *cmd);

/* ms_redirect */
int ms_open_at(int fd, const char *path, int oflag, int mode);
int ms_open_redirect_input(t_redir *redir);
int ms_open_redirect_output(t_redir *redir);
int	ms_open_redirect_append(t_redir *redir);
int	ms_redirect(t_redir *redir);

// 必要なし
int ms_duplicate_fd(int fd_from, int fd_into);

/* ms_redirect_heredoc */
int		ms_heredoc_signal_set(void);
t_list	*ms_read_heredoc(int fd, int pipe_fd, char *delimiter);
int		ms_redirect_heredoc(int io_number);
void	ms_heredoc_sigint_handler(int sig);

#endif
