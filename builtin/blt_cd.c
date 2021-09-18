/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 13:30:08 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/18 19:36:24 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

// 最初のgetcwdで失敗してもエラー出さない
// PWD がunsetされてたら参照できないので、移動前にgetcwdで保持しておく
// 第一引数のみ適用される、それ以降は無視される
int	blt_cd(t_shellvar *env, t_stree *tree, t_master *master)
{
	char		*old_pwd;

	if (tree != NULL && ft_strlen(tree->token) > 255)
	{
		blt_cd_print_error(master, tree->token, "File name too long");
		return (MS_BLT_FAIL);
	}
	old_pwd = getcwd(NULL, 0);
	if ((blt_cd_change_dir(env, tree, master) == MS_BLT_SUCC) && \
		blt_cd_update_pwd(env, tree, old_pwd) == MS_BLT_SUCC)
	{
		free(old_pwd);
		return (MS_BLT_SUCC);
	}
	free(old_pwd);
	return (MS_BLT_FAIL);
}

int	blt_cd_change_dir(t_shellvar *env, t_stree *tree, t_master *master)
{
	if (tree == NULL)
	{
		if (blt_cd_home(env, master) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	else
	{
		if (chdir(tree->token) == -1)
		{
			blt_cd_print_error(master, tree->token, strerror(errno));
			return (MS_BLT_FAIL);
		}
	}
	return (MS_BLT_SUCC);
}

int	blt_cd_home(t_shellvar *env, t_master *master)
{
	t_shellvar	*home_pos;

	home_pos = ms_search_key(env, "HOME");
	if (home_pos == NULL)
	{
		blt_cd_print_error(master, "HOME", "not set");
		return (MS_BLT_FAIL);
	}
	else if (chdir(home_pos->value) == -1)
	{
		blt_cd_print_error(master, home_pos->value, strerror(errno));
		return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

void	blt_cd_print_error(t_master *master, char *dirname, char *message)
{
	exec_error_prologue(master);
	if (dirname == NULL)
		perror("getcwd");
	else
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd(dirname, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(message, STDERR_FILENO);
	}
}
