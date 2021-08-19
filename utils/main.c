
#include "ms_utils.h"

// (構造体, io_number, type, path, command)
void	t_pushback(t_test *test, int io_number, int detail_type, char *path, char *cmd);
int		t_len(t_test *test);
t_list	*ms_redirect_heredoc(int fd, int pipe_fd, char *delimiter);
int		debug_fd;


void input_test(t_test *test)
{
	int	i;

	i = 0;
	while (i++ < 100)
		t_pushback(test, 0, REDIRECT_INPUT, "input.txt", "cat");
}

void pipe_test(t_test *test)
{
	int	i;

	i = 0;
	t_pushback(test, -1, -1, NULL, "ls");
	while (i++ < 100)
		t_pushback(test, -1, -1, NULL, "cat");
	t_pushback(test, -1, -1, NULL, "wc");
}

void output_test(t_test *test)
{
	int	i;

	i = 0;
	t_pushback(test, -1, -1, NULL, "ls");
//	while (i++ < 100)
		t_pushback(test, 1, REDIRECT_OUTPUT, "output.txt", "cat");
}

void append_test(t_test *test)
{
	int	i;

	i = 0;
	t_pushback(test, -1, -1, NULL, "ls");
	while (i++ < 100)
		t_pushback(test, 1, REDIRECT_APPEND, "append.txt", "cat");
}

void do_test(t_test *test, char **envp)
{
	test = test->next;
	int sz = t_len(test);
	while (test)
	{
		ms_execute_command(test, envp);
		test = test->next;
	}
	while (sz--)
		wait(NULL);
}

int heredoc_test(char *delimiter)
{
	t_list *lst;

	lst = ms_redirect_heredoc(1, 1, delimiter);
	while (lst != NULL)
	{
		printf("%s\n", (char *)lst->content);
		lst = lst->next;
	}
	return (0);
}

// redirection test
int main(int argc, char **argv, char **envp)
{
	t_test	*test;
//	t_test	*tmp;
	debug_fd = open("debug.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	test = calloc(1, sizeof(t_test));
//	pipe_test(test);
//	input_test(test);
//	output_test(test);
//	append_test(test);
	heredoc_test(argv[1]);

	do_test(test, envp);
	return (0);
}
