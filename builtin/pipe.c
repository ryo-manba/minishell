#include <libc.h>

int pipe_fd[2];
char *p = "Hello, dad!!\n";

void	do_child()
{
	printf("this is child\n");
	close(pipe_fd[0]);
	close(1);
	dup2(pipe_fd[1], 1);
	close(pipe_fd[1]);
	printf("hello\n"); // 標準出力(1)がpipe_fd[1]にリダイレクトされる。
}

void	do_parent()
{
	char c;
	int	status;

	printf("this is parent\n");
	close(pipe_fd[1]);
	close(0);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[0]);
	while ((c = getchar()) != EOF)
		putchar(c);
	wait(&status);
//	printf("%d\n", status);
}

int	main()
{
	int pid;

	if (pipe(pipe_fd) < 0){
		perror("pipe");
		return (1);
	}
	pid = fork();

	if (pid)
		do_parent();
	else
		do_child();

}
