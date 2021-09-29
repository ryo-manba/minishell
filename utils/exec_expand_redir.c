/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 20:24:37 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/29 23:03:48 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

static int	exec_expand_redir_loop(
		t_master *master, t_redir *rd, t_ex_state *es)
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
		if (ms_redirect(es, expanded_rd) == MS_EXEC_FAIL)
		{
			ms_check_fd_print_error(expanded_rd, master);
			if (destroy_needed)
				pa_destroy_redir(expanded_rd);
			return (MS_EXEC_FAIL);
		}
		if (destroy_needed)
			pa_destroy_redir(expanded_rd);
		rd = rd->next;
	}
	return (MS_EXEC_SUCC);
}

int	exec_expand_redirect(t_master *master, t_clause *clause)
{
	t_redir		*rd;
	t_ex_state	es;
	int			ret;

	ms_ex_init_state(&es, master);
	rd = clause->redir;
	ret = exec_expand_redir_loop(master, rd, &es);
	return (ret);
}
