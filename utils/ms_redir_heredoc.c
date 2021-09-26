/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_redir_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:09:07 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/26 23:35:27 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

int	ms_heredoc_child(t_redir **rd, t_ex_state *es, int pipefd[2])
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		ms_perror_exit("signal");
	if (close(pipefd[0]) == -1)
		ms_perror_exit("close");
	if (ms_heredoc_read_write(es, rd, pipefd[1]))
	{
		if (close(pipefd[1]) == -1)
			ms_perror_exit("close");
		exit(1);
	}
	exit(0);
}

int	ms_heredoc_parent(t_redir **rd, int pipefd[2])
{
	int	status;

	(*rd)->heredoc_fd = pipefd[0];
	if (close(pipefd[1]) == -1)
		ms_perror("close");
	wait(&status);
	if (WIFSIGNALED(status) && (WTERMSIG(status) == SIGINT))
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
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

int	ms_heredoc_rd(t_redir **rd, t_ex_state *es)
{
	pid_t	pid;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
	{
		ms_perror("pipe");
		return (1);
	}
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		ms_perror("signal");
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
	return (0);
}

int	ms_heredoc_loop(t_redir **rd, t_ex_state *es)
{
	int		ret;

	ret = 0;
	while (*rd)
	{
		if ((*rd)->redir_op == TI_LTLT)
		{
			if (ms_heredoc_rd(rd, es))
			{
				ret = 1;
				break ;
			}
		}
		*rd = (*rd)->next;
	}
	if (signal(SIGINT, ms_sigint_handler) == SIG_ERR)
		ms_perror("signal");
	return (ret);
}

int	ms_heredoc(t_clause **cl, t_ex_state *es)
{
	t_clause	*head_cl;
	t_redir		*head_rd;

	head_cl = *cl;
	while (*cl)
	{
		head_rd = (*cl)->redir;
		if (ms_heredoc_loop(&(*cl)->redir, es))
			return (1);
		(*cl)->redir = head_rd;
		(*cl) = (*cl)->next;
	}
	*cl = head_cl;
	return (0);
}
