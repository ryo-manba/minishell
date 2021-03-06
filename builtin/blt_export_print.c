/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_print.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka < rmatsuka@student.42tokyo.jp>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 18:00:05 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/27 10:40:53 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

static void	blt_putstr_with_escaping(char *str)
{
	size_t	n;

	while (*str)
	{
		if (ft_strchr_i("\"$", *str) >= 0)
		{
			ft_putchar_fd('\\', STDOUT_FILENO);
			ft_putchar_fd(*str, STDOUT_FILENO);
			str += 1;
		}
		else
		{
			n = 0;
			while (str[n] && ft_strchr_i("\"$", str[n]) < 0)
				n += 1;
			ft_putnstr_fd(str, n, STDOUT_FILENO);
			str += n;
		}
	}
}

static t_shellvar	*blt_copy_env(t_shellvar *env)
{
	t_shellvar	*tmp;
	t_shellvar	*copy;

	tmp = env;
	copy = NULL;
	while (tmp)
	{
		if (tmp->is_env)
			ms_env_add_back(&copy,
				ms_new_env(tmp->key, tmp->value, tmp->is_env));
		tmp = tmp->next;
	}
	return (copy);
}

static void	blt_print_export(t_shellvar *env)
{
	t_shellvar	*tmp;

	tmp = env;
	while (tmp)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		if (tmp->value == NULL)
			ft_putendl_fd(tmp->key, STDOUT_FILENO);
		else
		{
			ft_putstr_fd(tmp->key, STDOUT_FILENO);
			ft_putstr_fd("=\"", STDOUT_FILENO);
			blt_putstr_with_escaping(tmp->value);
			ft_putendl_fd("\"", STDOUT_FILENO);
		}
		tmp = tmp->next;
	}
}

void	blt_export_print_sorted_env(t_shellvar *env)
{
	t_shellvar	*copy;

	copy = blt_copy_env(env);
	blt_quick_sort(copy, ms_envlast(copy));
	blt_print_export(copy);
	ms_env_all_free(&copy);
}
