/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_redir_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 13:32:51 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/17 16:49:33 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

// fdの指定がない場合
int	ms_check_fd(char *fd)
{
	int64_t	num;
	int		i;

	if (fd == NULL)
		return (0);
	i = 0;
	num = 0;
	while (fd[i])
	{
		num = num * 10 + fd[i] - '0';
		if (num > INT_MAX)
			break ;
		i++;
	}
	if (MAX_FD <= num && num <= INT_MAX)
		return (OVER_FD);
	if (INT_MAX < num)
		return (OVER_INT);
	return (0);
}

int	ms_check_fd_print_error(t_redir *rd, t_master *master)
{
	int	err;

	err = 0;
	if (errno == EBADF || errno == EACCES)
	{
		if (errno == EBADF)
			err = ms_check_fd(rd->operand_left->token);
		ms_redir_print_error(master, err, rd->operand_left->token);
	}
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(rd->operand_right->token, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
	}
	return (errno);
}

// 文字列の種類
// EACCES: file_name
// OVER_FD: fd
void	ms_redir_print_error(t_master *master, int err, char *s)
{
	exec_error_prologue(master);
	if (err == EACCES)
	{
		ft_putstr_fd(s, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return ;
	}
	if (err == OVER_FD)
		ft_putstr_fd(s, STDERR_FILENO);
	if (err == OVER_INT)
		ft_putstr_fd("file descriptor out of range", STDERR_FILENO);
	ft_putendl_fd(": Bad file descriptor", STDERR_FILENO);
}
