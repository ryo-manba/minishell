/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 21:40:39 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/27 11:43:49 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

static size_t	exec_get_command_size(t_stree *tree)
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

static void	exec_check_path_exit(
		t_master *master, t_stree *expanded, char *path)
{
	if (g_ex_states == PERMISSION || g_ex_states == IS_A_DIR)
		exec_print_error_exit(master, g_ex_states, path);
	else if (path == NULL)
		exec_print_error_exit(master, g_ex_states, expanded->token);
}

// {"ls", "-l", "NULL"}
// Set the argument to this format.
static char	**exec_create_command(t_stree *tree)
{
	char	**command;
	size_t	i;
	t_stree	*tmp;

	i = exec_get_command_size(tree);
	command = (char **)malloc(sizeof(char *) * (i + 1));
	if (command == NULL)
		ms_perror_exit("malloc");
	tmp = tree;
	i = 0;
	while (tmp)
	{
		command[i] = ft_strdup(tmp->token);
		if (command[i] == NULL)
		{
			exec_all_free(command);
			ms_perror_exit("malloc");
		}
		tmp = tmp->right;
		i++;
	}
	command[i] = NULL;
	return (command);
}

static void	exec_exit_ifneed(t_master *master, t_stree *expanded,
	int is_command)
{
	(void)is_command;
	if (errno == ERRNO_EXECVE_FORMAT_ERROR)
		exit(0);
	if (errno == ERRNO_NO_SUCH_FILE)
		exec_print_error_exit(master, NO_SUCH_FILE, expanded->token);
	if (errno == ERRNO_NOT_A_DIR)
		exec_print_error_exit(master, NOT_A_DIR, expanded->token);
	exec_print_error_exit(master, CMD_NOT_FOUND, expanded->token);
}

// If '/' is included, run as is.
// If it is not included, find it in PATH.
void	exec_run_cmd_exit(t_master *master, t_stree *expanded, t_shellvar *var)
{
	char	*path;
	char	**env;
	int		is_command;

	is_command = 1;
	env = exec_restore_env(var);
	if (env == NULL)
		ms_perror_exit("malloc");
	if (ft_strchr_i(expanded->token, '/') != -1)
	{
		path = ft_strdup(expanded->token);
		if (path == NULL)
			ms_perror_exit("malloc");
		is_command = 0;
	}
	else
		path = exec_get_path(expanded->token, var);
	exec_check_path_exit(master, expanded, path);
	execve(path, exec_create_command(expanded), env);
	exec_all_free(env);
	exec_check_path_stat(path, is_command);
	exec_check_path_exit(master, expanded, path);
	free(path);
	exec_exit_ifneed(master, expanded, is_command);
}
