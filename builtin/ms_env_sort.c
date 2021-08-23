#include "ms_builtin.h"

// keyとvalueの両方をswapする
void	ms_swap_env(t_shellvar *env1, t_shellvar *env2)
{
	char *tmp_value;
	char *tmp_key;

	tmp_key = env1->key;
	tmp_value = env1->value;
	env1->key = env2->key;
	env1->value = env2->value;
	env2->key = tmp_key;
	env2->value = tmp_value;
}

t_shellvar *ms_partition(t_shellvar *first, t_shellvar *last)
{
	t_shellvar *pivot;
	t_shellvar *front;

	pivot = first; // 初期のpivotは先頭の要素
	front = first; // frontを進めていく
	while (front != NULL && front != last)
	{
		if (ft_strcmp(front->key, last->key) < 0) // 前のほうが後ろよりも小さかったら
		{
			pivot = first;
			ms_swap_env(first, front);
			first = first->next;
		}
		front = front->next;
	}
	ms_swap_env(first, last);
	return (pivot);
}

void	ms_quick_sort(t_shellvar *first, t_shellvar *last)
{
	t_shellvar *pivot;

	if (first == last)
	{
		return ;
	}
	pivot = ms_partition(first, last);
	if (pivot != NULL && pivot->next != NULL)
	{
		ms_quick_sort(pivot->next, last);
	}
	if (pivot != NULL && first != pivot)
	{
		ms_quick_sort(first, pivot);
	}
}
