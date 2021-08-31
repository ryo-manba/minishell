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

typedef struct s_test
{
	char	*cmd;
	bool	has_redirect;
	bool	is_builtin;
	int		io_number;
	int		detail_type;
	char	*file_path;
	struct s_test *next;
}	t_test;

/* ms_pipe.c */
int 	ms_first_pipe(int pipe_fd[2]);
int 	ms_last_pipe(int before_pipe[2]);
int 	ms_middle_pipe(int pipe_fd[2], int before_pipe[2]);
void 	ms_close_and_update_pipe(int pipe_fd[2], int before_pipe[2]);
int		ms_do_piping(t_clause *test, int pipe_fd[2], int before_pipe[2]);

/* ms_executer */
int	ms_executer(t_pipeline *pl, t_shellvar *var, t_ex_state *state);
int	ms_simple_command(t_shellvar *var, t_clause *clause);

/* ms_execute_command */
void	ms_execute_child(t_clause *test, int pipe_fd[2], int before_pipe[2], char **envp);
void	ms_execute_command(t_clause *test, char **envp);

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
