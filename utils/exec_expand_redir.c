/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 20:24:37 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/24 20:24:53 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

int	exec_expand_redir_loop(
		t_master *master, t_redir *rd, t_ex_state *es, int stdin_copy)
{
	t_redir	*expanded_rd;

	while (rd)
	{
		expanded_rd = ms_expand_a_redir(es, rd);
		if (!expanded_rd)
			return (MS_EXEC_FAIL);
		if (expanded_rd->redir_op == TI_LTLT)
		{
			if (dup2(stdin_copy, 0) == -1)
			{
				ms_perror("dup2");
				return (MS_EXEC_FAIL);
			}
		}
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
	int			stdin_copy;
	int			ret;

	ms_ex_init_state(&es, master);
	rd = clause->redir;
	stdin_copy = dup(STDIN_FILENO);
	if (stdin_copy == -1)
	{
		ms_perror("dup");
		return (MS_EXEC_FAIL);
	}
	ret = exec_expand_redir_loop(master, rd, &es, stdin_copy);
	if (close(stdin_copy) == -1)
	{
		ms_perror("close");
		ret = MS_EXEC_FAIL;
	}
	return (ret);
}
