#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

void do_child(char **s, char **envp)
{
//	if (execve(s[0], s, envp) < 0) // ここでプロセスが終了して0を返す
//	{
//		perror("execve");
//		exit(42);
//	}
	execlp("ls", "ls", NULL);
}

int main(int argc, char **argv, char **envp)
{
	char 	*s[3];
	pid_t	pid;
	int		status;
	int		file_fd;

	s[0] = argv[1];
	s[1] = argv[2];
	s[2] = NULL;
	file_fd = open("test.txt", O_RDWR);
	if (file_fd < 0)
	{
		perror("open");
		close(file_fd);
		return (1);
	}
	close(1); // 標準出力を閉じる
	dup2(file_fd, 1); // 複製して1をfile_fdにする
	close(file_fd);

	pid = fork();
	if (pid == 0)
		do_child(s, envp);
	wait(&status);
	printf("pid -> %d status -> %d\n", pid, WEXITSTATUS(status));

	return 0;
}

// ./a.out /bin/echo hello
