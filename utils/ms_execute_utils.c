/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:46 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/08 19:08:47 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

// コマンドの引数を数える
// $ "ls -l" = 2
size_t	ms_get_cmd_size(t_stree *tree)
{
	t_stree *tmp;
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

int	ms_check_fd(char *fd)
{
	int64_t	num;
	int	i;

	if (fd == NULL) // fdの指定がない場合
		return (0);
	i = 0;
	num = 0;
	while (fd[i])
	{
		num = num * 10 + fd[i] - '0';
		if (num > INT_MAX)
			break;
		i++;
	}
	if (MAX_FD <= num && num <= INT_MAX)
		return (OVER_FD);
	if (INT_MAX < num)
		return (OVER_INT);
	return (num);
}

void	ms_redir_print_error(int err, char *s)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (err == EACCES)
	{
		ft_putstr_fd(s, STDERR_FILENO); // file_name
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return ;
	}
	if (err == OVER_FD) // こっちの場合は数字も出す
	{
		ft_putstr_fd(s, STDERR_FILENO); // fd
	}
	if (err == OVER_INT)
	{
		ft_putstr_fd("file descriptor out of range", STDERR_FILENO);
	}
	ft_putendl_fd(": Bad file descriptor", STDERR_FILENO);
}

int	ms_check_and_print_error(t_redir *rd, int flag)
{
	if (flag != 0) // fdエラー
	{
		if (flag == EBADF || flag == EACCES)
		{
			if (flag == EBADF)
				flag = ms_check_fd(rd->operand_left->token);
			ms_redir_print_error(flag, rd->operand_left->token);
		}
		else
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putendl_fd(strerror(flag), STDERR_FILENO);
		}
		return (1);
	}
	return (0);
}
