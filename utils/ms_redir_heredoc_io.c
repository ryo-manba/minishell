/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_redir_heredoc_io.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/26 14:16:33 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/30 20:18:23 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

static int	ms_heredoc_read(t_list **lst, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, delimiter) == 0)
			break ;
		if (ft_lstpush_back(lst, line))
		{
			ms_perror("malloc");
			ft_lstclear(lst, free);
			g_ex_states = 1;
			return (1);
		}
	}
	return (0);
}

static int	ms_heredoc_write(t_ex_state *es, t_list *lst, int quoted, int fd)
{
	t_list	*tmp;
	t_list	*head;
	char	*expanded;

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
	return (MS_EXEC_SUCC);
}

// If you exit by entering ctrl+C, nothing will be done.
int	ms_heredoc_read_write(t_ex_state *es, t_redir **redir, int write_fd)
{
	t_list	*lst;
	int		quoted;

	lst = NULL;
	quoted = !!(*redir)->operand_right->quote_involved;
	if (ms_heredoc_read(&lst, (*redir)->operand_right->token) == MS_EXEC_FAIL)
		return (MS_EXEC_FAIL);
	if (ms_heredoc_write(es, lst, quoted, write_fd) == 1)
	{
		ft_lstclear(&lst, free);
		return (MS_EXEC_FAIL);
	}
	ft_lstclear(&lst, free);
	return (MS_EXEC_SUCC);
}

int	ms_redir_is_heredoc(t_redir *redir)
{
	if (redir)
	{
		if (redir->redir_op == TI_LTLT)
			return (1);
		if (redir->redir_op == TI_LTLTHYPHEN)
			return (1);
	}
	return (0);
}
