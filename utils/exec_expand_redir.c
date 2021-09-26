/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 20:24:37 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/26 14:35:37 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

int	exec_expand_redir_loop(
		t_master *master, t_redir *rd, t_ex_state *es, int stdin_copy)
{
	t_redir	*expanded_rd;

	(void)stdin_copy;
	while (rd)
	{
		expanded_rd = ms_expand_a_redir(es, rd);
		if (!expanded_rd)
			return (MS_EXEC_FAIL);
		if (ms_redirect(es, expanded_rd) == MS_EXEC_FAIL)
		{
			ms_check_fd_print_error(expanded_rd, master);
			pa_destroy_redir(expanded_rd);
			return (MS_EXEC_FAIL);
		}
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
	ret = exec_expand_redir_loop(master, rd, &es, 0);
	return (ret);
}
