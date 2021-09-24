/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_print.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 18:00:05 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/24 21:19:09 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

int	blt_check_escape(char *s)
{
	if (ft_strlen(s) == 2 && ft_strcmp(s, "\\n") == 0)
		return (1);
	if (ft_strlen(s) == 1 && ft_strchr_i("\"'`$\\", s[0]) != -1)
		return (1);
	return (0);
}

void	blt_putstr_with_escaping(char *str)
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

t_shellvar	*blt_copy_env(t_shellvar *env)
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

void	blt_print_export(t_shellvar *env)
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

void	blt_export_print_and_sort_env(t_shellvar *env)
{
	t_shellvar	*copy;

	copy = blt_copy_env(env);
	blt_quick_sort(copy, ms_envlast(copy));
	blt_print_export(copy);
	ms_env_all_free(&copy);
}
