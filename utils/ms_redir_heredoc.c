/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_redir_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:09:07 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/30 21:31:49 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

static int	ms_heredoc_child(t_redir *rd, t_ex_state *es, int pipefd[2])
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		ms_perror_exit("signal");
	if (close(pipefd[0]) == -1)
		ms_perror_exit("close");
	if (ms_heredoc_read_write(es, &rd, pipefd[1]))
	{
		if (close(pipefd[1]) == -1)
			ms_perror_exit("close");
		exit(1);
	}
	exit(0);
}

static int	ms_heredoc_parent(t_redir *rd, int pipefd[2])
{
	int	status;

	rd->heredoc_fd = pipefd[0];
	if (close(pipefd[1]) == -1)
		ms_perror("close");
	wait(&status);
	if (WIFSIGNALED(status) && (WTERMSIG(status) == SIGINT))
	{
		if (close(pipefd[0]) == -1)
			ms_perror("close");
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

static int	ms_heredoc_rd(t_redir *rd, t_ex_state *es)
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

static int	ms_heredoc_loop(t_redir *rd, t_ex_state *es)
{
	int		ret;
	t_redir	*expanded;

	ret = 0;
	while (rd)
	{
		if (ms_redir_is_heredoc(rd))
		{
			expanded = ms_expand_a_redir(es, rd);
			if (!expanded || ms_heredoc_rd(expanded, es))
			{
				ret = 1;
				pa_destroy_redir(expanded);
				break ;
			}
			rd->heredoc_fd = expanded->heredoc_fd;
			pa_destroy_redir(expanded);
		}
		rd = rd->next;
	}
	if (signal(SIGINT, ms_sigint_handler) == SIG_ERR)
		ms_perror("signal");
	return (ret);
}

int	ms_heredoc(t_pipeline *pl, t_ex_state *es)
{
	t_pipeline	*tmp_pl;
	t_clause	*tmp_cl;
	t_redir		*head_rd;

	tmp_pl = pl;
	while (tmp_pl)
	{
		tmp_cl = tmp_pl->clause;
		while (tmp_cl)
		{
			head_rd = tmp_cl->redir;
			if (ms_heredoc_loop(tmp_cl->redir, es))
				return (1);
			if (tmp_cl->stree && ms_heredoc(tmp_cl->stree->subshell, es))
				return (1);
			tmp_cl->redir = head_rd;
			tmp_cl = tmp_cl->next;
		}
		tmp_pl = tmp_pl->next;
	}
	return (0);
}
