#include "ms_utils.h"

volatile sig_atomic_t g_flag = 0;

// ctrl+Cが来たら標準入力をcloseすることでreadlineはNULLを返す
void	ms_heredoc_sigint_handler(int sig)
{
	g_flag = 1;
	close(STDIN_FILENO);
}

int	ms_heredoc_signal_set()
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
t_list	*ms_read_heredoc(int fd, int pipe_fd, char *delimiter)
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
		dup2(backup, STDIN_FILENO);
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

// heredocの場合
// < の特殊ケース
int	ms_redirect_heredoc(int io_number)
{
	int pipe_fd[2];

	pipe(pipe_fd);
	ms_read_heredoc(); // heredoc読み込み
	ms_expandable(); // 環境変数展開
	dup2(pipe_fd[1], 1);
	close(pipe_fd[1]);
	ms_write_heredoc(); // 展開したやつを改行区切りでパイプに書き込む
	dup2(pipe_fd[0], io_number);
	close(pipe_fd[0]);
	return (0);
}
