/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_env_sort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 17:34:28 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/06 17:59:52 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

t_shellvar	*blt_partition(t_shellvar *top, t_shellvar *last)
{
	t_shellvar	*pivot;
	t_shellvar	*front;

	pivot = top;
	front = top;
	while (front != NULL && front != last)
	{
		if (ft_strcmp(front->key, last->key) < 0)
		{
			pivot = top;
			blt_swap_env(top, front);
			top = top->next;
		}
		front = front->next;
	}
	blt_swap_env(top, last);
	return (pivot);
}

void	blt_quick_sort(t_shellvar *top, t_shellvar *last)
{
	t_shellvar	*pivot;

	if (top == last)
		return ;
	pivot = blt_partition(top, last);
	if (pivot != NULL && pivot->next != NULL)
		blt_quick_sort(pivot->next, last);
	if (pivot != NULL && top != pivot)
		blt_quick_sort(top, pivot);
}

void	blt_swap_env(t_shellvar *env1, t_shellvar *env2)
{
	char	*tmp_value;
	char	*tmp_key;

	tmp_key = env1->key;
	tmp_value = env1->value;
	env1->key = env2->key;
	env1->value = env2->value;
	env2->key = tmp_key;
	env2->value = tmp_value;
}
