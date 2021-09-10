/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_redir_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:09:07 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/10 11:33:19 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

volatile sig_atomic_t g_flag = 0;

// ctrl+Cが来たら標準入力をcloseすることでreadlineはNULLを返す
void	ms_heredoc_sigint_handler(int sig)
{
	(void)sig; // TOOD: 適切に使用
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
			ft_putendl_fd("do_something", STDERR_FILENO);
		close(backup_fd);
		ft_lstclear(lst, free);
	//	ex_status = 1;
		return ;
	}
	close(backup_fd);
	return ;
}

// 変数展開して出力する
int	ms_heredoc_write(t_list *lst, int quoted, int fd)
{
	t_list 		*tmp;
	t_shellvar	*env;
	char		*expanded;

	tmp = lst;
	if (quoted == 0) // クオートで囲まれていなかったら変数展開する
	{
		env = ms_create_env();
		while (tmp != NULL)
		{
			expanded = ex_ll_heredoc_line((char *)tmp->content, env);
			if (!expanded)
				return (1);
			free(tmp->content);
			tmp->content = expanded;
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
int	ms_redirect_heredoc(t_redir *redir)
{
	t_list	*lst;
	int		pipefd[2];
	int		quoted;

	lst = NULL;
	quoted = !!redir->operand_right->quote_involved;
	ms_heredoc_read(&lst, redir->operand_right->token); // 標準入力から読み取る
	if (lst == NULL) // Ctrl+Cで終了した場合は何もしない
	{
		close(STDIN_FILENO);
		return (1);
	}
	if (pipe(pipefd) == -1)
		return (1);
	if (dup2(pipefd[0], STDIN_FILENO) == -1) // この時点でstdinはパイプになる
		return (errno);
	if (close(pipefd[0]) == -1)
		return (errno);
	ms_heredoc_write(lst, quoted, pipefd[1]); // 展開したやつを改行区切りでパイプに書き込む
	return (0);
}
