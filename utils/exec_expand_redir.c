/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 20:24:37 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/10/01 15:11:34 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

static int	ms_perror_return(char *s)
{
	ms_perror(s);
	return (1);
}

static int	ms_is_duplicate_backup(int backup_fd[3], char *fd)
{
	int	i;
	int	need_fd;
	int	new_fd;

	if (backup_fd == NULL || fd == NULL)
		return (0);
	i = -1;
	need_fd = ft_atoi(fd);
	while (++i < 3)
	{
		if (backup_fd[i] == need_fd)
		{
			new_fd = dup(backup_fd[i]);
			if (new_fd == -1)
				ms_perror_return("dup");
			if (close(backup_fd[i]) == -1)
				ms_perror_return("close");
			backup_fd[i] = new_fd;
		}
	}
	return (0);
}

static int	exec_redirect_and_check(t_master *master,
		t_redir *expanded_rd, int destroy_needed)
{
	if (ms_redirect(expanded_rd) == MS_EXEC_FAIL)
	{
		ms_check_fd_print_error(expanded_rd, master);
		if (destroy_needed)
			pa_destroy_redir(expanded_rd);
		return (MS_EXEC_FAIL);
	}
	if (destroy_needed)
		pa_destroy_redir(expanded_rd);
	return (MS_EXEC_SUCC);
}

static int	exec_expand_redir_loop(
		t_master *master, t_redir *rd, t_ex_state *es, int backup_fd[3])
{
	t_redir	*expanded_rd;
	int		destroy_needed;

	while (rd)
	{
		destroy_needed = !ms_redir_is_heredoc(rd);
		if (!ms_redir_is_heredoc(rd))
			expanded_rd = ms_expand_a_redir(es, rd);
		else
			expanded_rd = rd;
		if (!expanded_rd)
			return (MS_EXEC_FAIL);
		if (backup_fd && expanded_rd->operand_left && \
			ms_is_duplicate_backup(backup_fd, expanded_rd->operand_left->token))
			return (MS_EXEC_FAIL);
		if (exec_redirect_and_check(master, expanded_rd, destroy_needed))
			return (MS_EXEC_FAIL);
		rd = rd->next;
	}
	return (MS_EXEC_SUCC);
}

int	exec_expand_redirect(t_master *master, t_clause *clause, int backup_fd[3])
{
	t_redir		*rd;
	t_ex_state	es;
	int			ret;

	ms_ex_init_state(&es, master);
	rd = clause->redir;
	ret = exec_expand_redir_loop(master, rd, &es, backup_fd);
	return (ret);
}
