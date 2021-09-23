/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:46 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/23 12:18:18 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

void	exec_check_path_exit(t_master *master, t_stree *expanded, char *path)
{
	if (g_ex_states == PERMISSION || g_ex_states == IS_A_DIR)
		exec_print_error_exit(master, g_ex_states, path);
	else if (path == NULL)
		exec_print_error_exit(master, g_ex_states, expanded->token);
}

// コマンドの引数を数える
// $ "ls -l" = 2
size_t	exec_get_command_size(t_stree *tree)
{
	t_stree	*tmp;
	size_t	sz;

	tmp = tree;
	sz = 0;
	while (tmp != NULL)
	{
		tmp = tmp->right;
		sz++;
	}
	return (sz);
}

// waitで子プロセスの回収とexitstatusの更新
void	exec_set_signal_wait(pid_t pid)
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		ms_perror_exit("signal");
	exec_update_exitstatus(pid);
	while (wait(NULL) > 0)
		;
	if (signal(SIGINT, ms_sigint_handler) == SIG_ERR)
		ms_perror_exit("signal");
}

int	exec_out(int rv, t_stree *expanded)
{
	pa_destroy_stree(expanded);
	return (rv);
}
