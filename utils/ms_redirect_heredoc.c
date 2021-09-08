/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_redirect_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:09:07 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/08 19:09:08 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

volatile sig_atomic_t g_flag = 0;

// ctrl+Cが来たら標準入力をcloseすることでreadlineはNULLを返す
void	ms_heredoc_sigint_handler(int sig)
{
	g_flag = 1;
	ft_putchar_fd('\n', STDOUT_FILENO);
	close(STDIN_FILENO);
}

int	ms_heredoc_signal_set(void)
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
void ms_heredoc_read(t_list **lst, char *delimiter)
{
	char *line;
	int	backup_fd;

	backup_fd = dup(STDIN_FILENO);
	ms_heredoc_signal_set();
	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, delimiter) == 0 || g_flag == 1)
			break ;
		ft_lstpush_back(lst, line);
	}
	if (g_flag == 1) // ctrl-cで終わった場合終了ステータスは1になる
	{
		if (dup2(backup_fd, STDIN_FILENO) == -1)// dup2失敗した場合、どうするか
			do_something();
		close(backup_fd);
		ft_lstclear(lst, free);
	//	ex_status = 1;
		return (NULL);
	}
	close(backup_fd);
	return (lst);
}

// 変数展開して出力する
int	ms_heredoc_write(t_list *lst, int quoted, int fd)
{
	t_list *tmp;

	tmp = lst;
	if (quoted == 0) // クオートで囲まれていなかったら変数展開する
	{
		while (tmp != NULL)
		{
			ms_expander((char *)lst->content);
			tmp = tmp->next;
		}
		tmp = lst;
	}
	while (tmp)
	{
		ft_putendl_fd(lst->content, fd);
		tmp = tmp->next;
	}
	close(fd);
	return (0);
}

// << EOT << 'EOT' << "EOT" この判定どうするか
int	ms_redirect_heredoc(t_redir *redir, int quoted)
{
	t_list	**lst;
	int		pipefd[2];

	lst = (t_list **)malloc(sizeof(t_list *));
	if (lst == NULL)
	{
		ms_print_perror("malloc");
	}
	ms_heredoc_read(lst, redir->operand_right->token); // 標準入力から読み取る
	if (lst == NULL) // Ctrl+Cで終了した場合は何もしない
	{
		close(STDIN_FILENO);
		return (1);
	}
	if (pipe(pipefd) == -1);
		return (1);
	dup2(pipefd[0], STDIN_FILENO); // この時点でstdinはパイプになる
	close(pipefd[0]);
	ms_heredoc_write(lst, quoted, pipefd[1]); // 展開したやつを改行区切りでパイプに書き込む
	return (0);
}
