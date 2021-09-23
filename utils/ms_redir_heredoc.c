/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_redir_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:09:07 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/23 21:50:01 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

volatile sig_atomic_t	g_ex_states;

int	ms_heredoc_check_states(t_list **lst, int backup_fd)
{
	if (g_ex_states == 1)
	{
		if (dup2(backup_fd, STDIN_FILENO) == -1)
			ms_perror("dup2");
		if (close(backup_fd) == -1)
			ms_perror("close");
		ft_lstclear(lst, free);
		return (MS_EXEC_SUCC);
	}
	if (close(backup_fd) == -1)
		ms_perror("close");
	return (MS_EXEC_SUCC);
}

// ctrl+C    -> Output a newline and exit
// ctrl+D    -> exit
// delemiter -> exit
int	ms_heredoc_read(t_list **lst, char *delimiter)
{
	char	*line;
	int		backup_fd;

	backup_fd = dup(STDIN_FILENO);
	if (backup_fd == -1)
	{
		ms_perror("dup");
		return (MS_EXEC_FAIL);
	}
	if (ms_heredoc_signal_set())
		return (MS_EXEC_FAIL);
	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, delimiter) == 0 || g_ex_states == 1)
			break ;
		if (ft_lstpush_back(lst, line))
		{
			ms_perror("malloc");
			g_ex_states = 1;
			break ;
		}
	}
	return (ms_heredoc_check_states(lst, backup_fd));
}

int	ms_heredoc_write(t_ex_state *es, t_list *lst, int quoted, int fd)
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
			expanded = ex_ll_heredoc_line(es, (char *)tmp->content);
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

// If you exit by entering ctrl+C, nothing will be done.
int	ms_redirect_heredoc(t_ex_state *es, t_redir *redir)
{
	t_list	*lst;
	int		pipefd[2];
	int		quoted;

	lst = NULL;
	quoted = !!redir->operand_right->quote_involved;
	if (ms_heredoc_read(&lst, redir->operand_right->token) == MS_EXEC_FAIL)
		return (MS_EXEC_FAIL);
	if (pipe(pipefd) == -1)
		return (MS_EXEC_FAIL);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		return (MS_EXEC_FAIL);
	if (close(pipefd[0]) == -1)
		return (MS_EXEC_FAIL);
	ms_heredoc_write(es, lst, quoted, pipefd[1]);
	return (MS_EXEC_SUCC);
}
