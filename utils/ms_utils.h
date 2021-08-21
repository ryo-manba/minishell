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
void ms_first_pipe(int pipe_fd[2]);
void ms_last_pipe(int before_pipe[2]);
void ms_middle_pipe(int pipe_fd[2], int before_pipe[2]);
void ms_close_and_update_pipe(int pipe_fd[2], int before_pipe[2]);
char *ms_search_execution_path(DIR *dir, char *cmd, char *path);
char *ms_get_path(char *cmd);
int	ms_do_piping(t_clause *test, int pipe_fd[2], int before_pipe[2]);
void	ms_execute_child(t_clause *test, int pipe_fd[2], int before_pipe[2], char **envp);
void	ms_execute_command(t_clause *test, char **envp);

/* ms_redirect.c */
int ms_open_at(int fd, const char *path, int oflag, int mode);
int ms_duplicate_fd(int fd_from, int fd_into);
int ms_open_redirect_input(int io_number, const char *path);
int ms_open_redirect_output(int io_number, const char *path);
int	ms_open_redirect_append(int io_number, const char *path);
int	ms_redirect(int io_number, const char *path, int  detail_type);

static void	ms_heredoc_sigint_handler(int sig);
int	ms_heredoc_signal_set(void);
t_list	*ms_redirect_heredoc(int fd, int pipe_fd, char *delimiter);


#endif