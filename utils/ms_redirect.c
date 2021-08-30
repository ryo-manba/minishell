#include "ms_utils.h"

// [concrete functions for redirection]
// opens a file at `path` and binds to `fd`.
// the function closes `fd` before opening `path`.
int ms_open_at(int fd, const char *path, int oflag, int mode)
{
	int	open_fd;

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

// [abstract functions for redirection]
// open file for input-redirection [io_number]< path
// if io_number is not specified, STDIN_FILENO will be used for default value.
int ms_open_redirect_input(t_redir *redir)
{
	const char *path = redir->operand_right->token;
	int			io_number;

	if (redir->operand_left == NULL)
		io_number = STDIN_FILENO;
	else
		io_number = ft_atoi(redir->operand_left->token);
	return (ms_open_at(io_number, path, O_RDONLY, -1));
}

int ms_open_redirect_output(t_redir *redir)
{
	const char *path = redir->operand_right->token;
	int			io_number;

	if (redir->operand_left == NULL)
		io_number = STDOUT_FILENO;
	else
		io_number = ft_atoi(redir->operand_left->token);
	return (ms_open_at(io_number, path, O_WRONLY | O_CREAT | O_TRUNC, 0666));
}

int	ms_open_redirect_append(t_redir *redir)
{
	const char *path = redir->operand_right->token;
	int			io_number;

	if (redir->operand_left == NULL)
		io_number = STDOUT_FILENO;
	else
		io_number = ft_atoi(redir->operand_left->token);
	return (ms_open_at(io_number, path, O_WRONLY | O_CREAT | O_APPEND, 0666));
}

int	ms_redirect(t_redir *redir)
{
	if (redir->redir_op == TI_LT) // <
	{
		return (ms_open_redirect_input(redir));
	}
	else if (redir->redir_op == TI_GT) // >
	{
		return (ms_open_redirect_output(redir));
	}
	else if (redir->redir_op == TI_GTGT) // >>
	{
		return (ms_open_redirect_append(redir));
	}
	else if (redir->redir_op == TI_LTLT) // <<
	{
		return (ms_redirect_heredoc(redir));
	}
	return (1);
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
