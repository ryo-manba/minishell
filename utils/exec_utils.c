/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:08:46 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/10 22:55:23 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

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
