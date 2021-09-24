/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 21:40:39 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/23 22:01:08 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

static void	exec_exit_ifneeed(t_master *master, t_stree *expanded,
	int is_relative)
{
	(void)is_relative;
	if (errno == ERRNO_EXECVE_FORMAT_ERROR)
		return ;
	if (errno == ERRNO_NO_SUCH_FILE)
		exec_print_error_exit(master, NO_SUCH_FILE, expanded->token);
	if (errno == ERRNO_NOT_A_DIR)
		exec_print_error_exit(master, NOT_A_DIR, expanded->token);
	exec_print_error_exit(master, CMD_NOT_FOUND, expanded->token);
}

// '/' が含まれてる場合はそのまま実行する
// ない場合はPATHから探す
void	exec_run_cmd_exit(t_master *master, t_stree *expanded, t_shellvar *var)
{
	char	*path;
	char	**env;
	int		is_relative;

	is_relative = 1;
	env = exec_restore_env(var);
	if (env == NULL)
		ms_perror_exit("malloc");
	if (ft_strchr_i(expanded->token, '/') != -1)
	{
		path = ft_strdup(expanded->token);
		if (path == NULL)
			ms_perror_exit("malloc");
		is_relative = 0;
	}
	else
		path = exec_get_path(expanded->token, var);
	exec_check_path_exit(master, expanded, path);
	execve(path, exec_create_command(expanded), env);
	exec_all_free(env);
	exec_check_path(path, is_relative);
	exec_check_path_exit(master, expanded, path);
	free(path);
	exec_exit_ifneeed(master, expanded, is_relative);
}
