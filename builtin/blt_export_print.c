/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_print.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 18:00:05 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/08 17:40:07 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

// ソートするため環境変数のコピーを作る
t_shellvar	*blt_copy_env(t_shellvar *env)
{
	t_shellvar	*tmp;
	t_shellvar	*copy;

	tmp = env;
	copy = NULL;
	while (tmp)
	{
		ms_env_add_back(&copy, ms_new_env(tmp->key, tmp->value));
		tmp = tmp->next;
	}
	return (copy);
}

// sort済みの環境変数を出力する
// export a="\n"　declare -x a="\\n"にする
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
			ft_putstr_fd('=\"', STDOUT_FILENO);
			if (ft_strncmp(tmp->value, "\\", 1) == 0)
				ft_putchar_fd('\\', STDOUT_FILENO);
			ft_putstr_fd(tmp->value, STDOUT_FILENO);
			ft_putendl_fd("\"", STDOUT_FILENO);
		}
		tmp = tmp->next;
	}
}

void	blt_print_sort_env(t_shellvar *env)
{
	t_shellvar	*copy;

	copy = blt_copy_env(env);
	blt_quick_sort(copy, ms_envlast(copy));
	blt_print_export(copy);
	ms_env_all_free(copy);
}
