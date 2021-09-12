/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 13:30:08 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/12 19:12:57 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

// 最初のgetcwdで失敗してもエラー出さない
// PWD がunsetされてたら参照できないので、移動前にgetcwdで保持しておく
// 第一引数のみ適用される、それ以降は無視される
int	blt_cd(t_shellvar *env, t_stree *tree)
{
	char		*old_pwd;

	if (tree != NULL && ft_strlen(tree->token) > 255)
	{
		blt_cd_print_error(tree->token, "File name too long");
		return (MS_BLT_FAIL);
	}
	old_pwd = getcwd(NULL, 0);
	if (blt_cd_change_dir(env, tree) == MS_BLT_SUCC && \
		blt_cd_update_pwd(env, old_pwd) == MS_BLT_SUCC)
	{
		free(old_pwd);
		return (MS_BLT_SUCC);
	}
	free(old_pwd);
	return (MS_BLT_FAIL);
}

int	blt_cd_change_dir(t_shellvar *env, t_stree *tree)
{
	if (tree == NULL)
	{
		if (blt_cd_home(env) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	else
	{
		if (chdir(tree->token) == -1)
		{
			blt_cd_print_error(tree->token, strerror(errno));
			return (MS_BLT_FAIL);
		}
	}
	return (MS_BLT_SUCC);
}

int	blt_cd_home(t_shellvar *env)
{
	t_shellvar	*home_pos;

	home_pos = ms_search_key(env, "HOME");
	if (home_pos == NULL)
	{
		blt_cd_print_error("HOME", "not set");
		return (MS_BLT_FAIL);
	}
	else if (chdir(home_pos->value) == -1)
	{
		blt_cd_print_error(home_pos->value, strerror(errno));
		return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

void	blt_cd_print_error(char *dirname, char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
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

// PWD, OLDPWDが unsetされている場合は新しく作らない
int	blt_cd_update_pwd(t_shellvar *env, char *old_pwd)
{
	char	*pwd;

	errno = 0;
	pwd = getcwd(NULL, 0);
	if (errno != 0)
	{
		blt_cd_print_error(NULL, NULL);
		return (MS_BLT_FAIL);
	}
	if (old_pwd == NULL)
	{
		free(pwd);
		return (MS_BLT_SUCC);
	}
	if (blt_search_and_update_env(env, "OLDPWD", old_pwd) == MS_BLT_FAIL && \
		blt_search_and_update_env(env, "PWD", pwd) == MS_BLT_FAIL)
		return (MS_BLT_FAIL);
	return (MS_BLT_SUCC);
}
