/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_redir_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:09:07 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/10 23:19:17 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

volatile sig_atomic_t	g_flag = 0;

// ctrl+C    -> 改行を出力して終了
// ctrl+D    -> 終了
// delemiter -> 終了
// ctrl-cで終わった場合終了ステータスは1になる
int	ms_heredoc_read(t_list **lst, char *delimiter)
{
	char	*line;
	int		backup_fd;

	backup_fd = dup(STDIN_FILENO);
	ms_heredoc_signal_set();
	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, delimiter) == 0 || g_flag == 1)
			break ;
		ft_lstpush_back(lst, line);
	}
	if (g_flag == 1)
	{
		if (dup2(backup_fd, STDIN_FILENO) == -1)
			ms_print_perror("dup2");
		close(backup_fd);
		ft_lstclear(lst, free);
		return (MS_EXEC_FAIL);
	}
	close(backup_fd);
	return (MS_EXEC_SUCC);
}

// ctrl+Cが来たら標準入力をcloseすることでreadlineはNULLを返す
void	ms_heredoc_sigint_handler(int sig)
{
	(void)sig;
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

// 変数展開して出力する
// クオートで囲まれていなかったら変数展開する
int	ms_heredoc_write(t_list *lst, t_shellvar *var, int quoted, int fd)
{
	t_list		*tmp;
	t_list		*head;
	char		*expanded;

	tmp = lst;
	head = tmp;
	if (quoted == 0)
	{
		while (tmp != NULL)
		{
			expanded = ex_ll_heredoc_line((char *)tmp->content, var);
			if (!expanded)
				return (MS_EXEC_FAIL);
			free(tmp->content);
			tmp->content = expanded;
			tmp = tmp->next;
		}
	}
	while (head)
	{
		ft_putendl_fd(head->content, fd);
		head = head->next;
	}
	close(fd);
	return (MS_EXEC_SUCC);
}

// Ctrl+Cで終了した場合は何もしない
// stdinをパイプにして展開したやつを改行区切りでパイプに書き込む
int	ms_redirect_heredoc(t_redir *redir, t_shellvar *var)
{
	t_list	*lst;
	int		pipefd[2];
	int		quoted;

	lst = NULL;
	quoted = !!redir->operand_right->quote_involved;
	if (ms_heredoc_read(&lst, redir->operand_right->token) == MS_EXEC_FAIL)
		return (MS_EXEC_FAIL);
	if (lst == NULL)
	{
		close(STDIN_FILENO);
		return (MS_EXEC_FAIL);
	}
	if (pipe(pipefd) == -1)
		return (MS_EXEC_FAIL);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		return (MS_EXEC_FAIL);
	if (close(pipefd[0]) == -1)
		return (MS_EXEC_FAIL);
	ms_heredoc_write(lst, var, quoted, pipefd[1]);
	return (MS_EXEC_SUCC);
}
