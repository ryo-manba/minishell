#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/errno.h>
#include <string.h>

extern int errno;

// [concrete functions for redirection]

// opens a file at `path` and binds to `fd`.
// the function closes `fd` before opening `path`.
int ms_open_at(int fd, const char *path, int oflag, int mode)
{
	int	auto_fd;
	int	rv;

	rv = close(fd);
	printf("close returned: %d\n", rv);
	rv = open(path, oflag, mode);
	printf("open returned: %d\n", rv);
	if (rv < 0)
		return (rv);
	auto_fd = rv;
	rv = dup2(auto_fd, fd);
	if (rv < 0)
		return (rv);
	close(auto_fd);
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
	return (ms_open_at(io_number, path, O_RDONLY, 0));
}

int ms_open_redirect_output(int io_number, const char *path)
{
	if (io_number == -1)
		io_number = STDOUT_FILENO;
	// TODO: ファイルが存在する場合、O_CREATはダメ
	return (ms_open_at(io_number, path, O_EXCL, 0));
}

int main(int argc, char **argv)
{
	int fd = ms_open_redirect_output(100, "/dev/stdout");
	printf("%s\n", strerror(errno));
	dprintf(fd, "print into FD%d\n", fd);
	printf("print into FD%d\n", fd);
}
