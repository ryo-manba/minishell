#include "ms_utils.h"

volatile sig_atomic_t g_flag = 0;

// ctrl+Cが来たら標準入力をcloseすることでreadlineはNULLを返す
static void	ms_heredoc_sigint_handler(int sig)
{
	g_flag = 1;
	close(STDIN_FILENO);
}

static int	ms_heredoc_signal_set()
{
	if (signal(SIGINT, ms_heredoc_sigint_handler) == SIG_ERR)
		return (1);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (1);
	return (0);
}

// ctrl+C    -> 改行を出力して終了
// ctrl+D    -> 終了
// delemiter -> 終了
t_list	*ms_redirect_heredoc(int fd, int pipe_fd, char *delimiter)
{
	char *line;
	int	backup;
	t_list	*lst;
	t_list	*tmp;

	backup = dup(STDIN_FILENO);
	ms_heredoc_signal_set();
	lst = ft_lstnew(NULL); // ダミーノード
	while (g_flag == 0)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, delimiter) == 0)
			break ;
		ft_lstpush_back(&lst, line);
	}
	if (g_flag == 1)
	{
		dup2(backup, 0);
		close(backup);
		ft_lstclear(&lst, free);
		return (NULL);
	}
	tmp = lst;
	lst = lst->next;
	free(tmp);
	close(backup);
	return (lst);
}

// [concrete functions for redirection]
// opens a file at `path` and binds to `fd`.
// the function closes `fd` before opening `path`.
int ms_open_at(int fd, const char *path, int oflag, int mode)
{
	int	open_fd; // close(read)

	if (mode == -1) // stdin
		open_fd = open(path, oflag);
	else
		open_fd = open(path, oflag, mode);
	if (open_fd < 0)
		return (-1);
	close(fd);	// openの前にすると読み込みに失敗する
	if (dup2(open_fd, fd) == -1)
		return (-1);
	close(open_fd);
	return (fd);
}

// duplicates fd_from into fd_into, or closes fd_into.
// if fd_from < 0: close
// otherwise: duplicate
int ms_duplicate_fd(int fd_from, int fd_into)
{
	int	rv;

	rv = close(fd_into);
	if (fd_from >= 0)
		rv = dup2(fd_from, fd_into);
	return (rv);
}

// [abstract functions for redirection]
// open file for input-redirection [io_number]< path
// if io_number is not specified, STDIN_FILENO will be used for default value.
int ms_open_redirect_input(int io_number, const char *path)
{
	return (ms_open_at(io_number, path, O_RDONLY, -1));
}

int ms_open_redirect_output(int io_number, const char *path)
{
	return (ms_open_at(io_number, path, O_WRONLY | O_CREAT | O_TRUNC, 0666));
}

int	ms_open_redirect_append(int io_number, const char *path)
{
	return (ms_open_at(io_number, path, O_WRONLY | O_CREAT | O_APPEND, 0666));
}

// heredocの場合どうするか
int	ms_open_redirect_heredoc()
{
	return (1);
}

//void	ms_redirect(int io_number, const char *path, t_word_list *lst)
int	ms_redirect(int io_number, const char *path, int  detail_type)
{
	if (detail_type == REDIRECT_INPUT)
	{
		return (ms_open_redirect_input(io_number, path));
	}
	else if (detail_type == REDIRECT_OUTPUT)
	{
		return (ms_open_redirect_output(io_number, path));
	}
	else if (detail_type == REDIRECT_APPEND)
	{
		return (ms_open_redirect_append(io_number, path));
	}
	else if (detail_type == TYPE_HEREDOC)
	{
//		return (ms_open_redirect_heredoc(io_number));
	}
	return (-1);
}
