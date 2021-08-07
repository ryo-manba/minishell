#include <libc.h>
#include <dirent.h>
#include "libft.h"

int pipe_fd[2];

char	*get_path(char *cmd)
{
	char	*path;
	char	**split_path;
	char	*tmp;
	char	*path_name;
	int		i;
	DIR		*dir;
	struct dirent *dirent;

	path = getenv("PATH");
	split_path = ft_split(path, ':');
	i = -1;
	while (split_path[++i])
	{
		dir = opendir(split_path[i]); // 権限がない場合もopendirに失敗する
		if (dir == NULL)
			continue ;
		while (1) 	// opendirに失敗したままreaddirするとsegfalutになる
		{
			dirent = readdir(dir);
			if (dirent == NULL)
				break ;
			if (ft_strcmp(dirent->d_name, cmd) == 0)
			{
				printf("d_name -> %s cmd ->  %s\n", dirent->d_name, cmd);
				closedir(dir);
				tmp = ft_strjoin(split_path[i], "/");
				path_name = ft_strjoin(tmp, cmd);
				free(tmp);
				return (path_name);
			}
		}
		closedir(dir);
	}
	return (NULL);
}

void	do_child(char *file, char *cmd, char **envp)
{
	int	fd;
	char	*path;

	printf("this is child\n");
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		exit(1);
	}
	close(pipe_fd[0]);
	close(1);
	dup2(pipe_fd[1], 1);
	close(pipe_fd[1]);
	dup2(fd, 0);
	path = get_path(cmd);
	execve(path, &cmd, envp);
	exit(0);
	printf("hello\n"); // 標準出力(1)がpipe_fd[1]にリダイレクトされる。
}

void	do_parent(char *file, char *cmd, char **envp)
{
	char c;
	int	status;
	int	fd;
	char	*path;

	wait(&status);
	printf("this is parent\n");
	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
	{
		perror("open");
		exit(1);
	}
	close(pipe_fd[1]);
	close(0);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[0]);
	dup2(fd, 1);
	path = get_path(cmd);
	execve(path, &cmd, envp);
	printf("%d\n", status);
	exit(1);
}

int	main(int argc, char **argv, char **envp)
{
	int pid;
	int	status;

	char *infile = argv[1];
	char *cmd = argv[2];
	char *cmd2 = argv[3];
	char *outfile = argv[4];

	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
//		do_child(infile, cmd, envp);
		execve(get_path(cmd), &cmd, envp);

	}
	else
	{
		wait(&status);
		sleep(1);
		//		printf("%d\n", status);
	}
//		do_parent(outfile, cmd2, envp);
}
