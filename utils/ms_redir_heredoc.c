/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_redir_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:09:07 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/26 14:19:34 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

int	ms_heredoc_child(t_redir **rd, t_ex_state *es, int pipefd[2])
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		ms_perror_exit("signal");
	close(pipefd[0]);
	if (ms_heredoc_read_write(es, rd, pipefd[1])) // 予期せぬエラーの場合
	{
		close(pipefd[1]);
		exit(1);
	}
	close(pipefd[1]);
	exit(0);
}

int	ms_heredoc_parent(t_redir **rd, int pipefd[2])
{
	int	status;

	(*rd)->heredoc_fd = pipefd[0];
	close(pipefd[1]);
	wait(&status);
	if (WIFSIGNALED(status) && (WTERMSIG(status) == SIGINT))
	{
//		ft_putchar_fd('\n', STDOUT_FILENO);
		g_ex_states = 1;
		return (1);
	}
	else
	{
		g_ex_states = WEXITSTATUS(status);
		if (g_ex_states != 0)
			return (1);
	}
	return (0);
}

int	ms_heredoc_rd_loop(t_redir **rd, t_ex_state *es)
{
	int		pipefd[2];
	pid_t	pid;

	while (*rd)
	{
		if ((*rd)->redir_op == TI_LTLT)
		{
			if (pipe(pipefd) == -1)
			{
				ms_perror("pipe");
				return (1);
			}
			pid = fork();
			if (pid == -1)
			{
				ms_perror("fork");
				return (1);
			}
			if (pid == 0)
				ms_heredoc_child(rd, es, pipefd);
			else
			{
				if (ms_heredoc_parent(rd, pipefd))
					return (1);
			}
		}
		*rd = (*rd)->next;
	}
	return (0);
}

int	ms_heredoc_pipe_loop(t_clause **cl, t_ex_state *es)
{
	t_redir	*head;

	while (*cl)
	{
		head = (*cl)->redir;
		if (ms_heredoc_rd_loop(&(*cl)->redir, es))
			return (1);
		(*cl)->redir = head;
		(*cl) = (*cl)->next;
	}
	return (0);
}

int	ms_heredoc(t_clause **cl, t_ex_state *es)
{
	t_clause *head;

	head = *cl;
	if (ms_heredoc_pipe_loop(cl, es))
		return (1);
	*cl = head;
	return (0);
}
