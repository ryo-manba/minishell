#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/errno.h>
#include <string.h>
#include "../lexer/lexer.h"

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
	if (dup2(open_fd, fd) == -1) // dup2(3,0)
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
	if (io_number == -1)
		io_number = STDIN_FILENO;
	return (ms_open_at(io_number, path, O_RDONLY, -1));
}

int ms_open_redirect_output(int io_number, const char *path)
{
	if (io_number == -1) // これはエラーでは？
		io_number = STDOUT_FILENO;
	return (ms_open_at(io_number, path, O_WRONLY | O_CREAT | O_TRUNC, 0666));
}

int	ms_open_redirect_append(int io_number, const char *path)
{
	if (io_number == -1)
		io_number = STDOUT_FILENO;
	return (ms_open_at(io_number, path, O_WRONLY | O_CREAT | O_APPEND, 0666));
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
	return (-1);
}

int	main()
{
	setvbuf(stdout, NULL, _IONBF, 0);
	int in, out, append, copy = dup(1);

	append = ms_redirect(1, "test1", REDIRECT_APPEND);
	printf("append: %d\n", append);
	out = ms_redirect(1, "test10", REDIRECT_OUTPUT);
	printf("out: %d\n", out);
//	in = ms_redirect(1, "test2", REDIRECT_INPUT);
//	printf("in: %d\n", in);
	dup2(copy, 1);
	puts("copy");
	return (0);
}
