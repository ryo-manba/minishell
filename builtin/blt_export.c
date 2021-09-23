/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 18:00:12 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/23 20:53:11 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

int	blt_append_or_join_env(t_shellvar *var, char *key_value[2])
{
	t_shellvar	*key_pos;

	key_pos = ms_search_key(var, key_value[KEY]);
	if (key_pos == NULL)
	{
		if (blt_append_env(var, key_value[KEY], key_value[VALUE]))
			return (MS_BLT_FAIL);
	}
	else if (key_value[VALUE] != NULL)
	{
		if (blt_join_env(key_pos, key_value) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

int	blt_check_and_export(t_stree *tree, t_shellvar *var, char *key_value[2])
{
	int32_t		equal_idx;
	t_shellvar	*tmp;

	tmp = var;
	equal_idx = ft_strchr_i(tree->token, '=');
	if (equal_idx != 0 && tree->token[equal_idx - 1] == '+')
	{
		if (blt_append_or_join_env(tmp, key_value) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	else
	{
		if (blt_append_or_update_env(
				tmp, key_value[KEY], key_value[VALUE]) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

int	blt_export_env(t_shellvar *var, t_stree *tree, t_master *master)
{
	char	*key_value[2];
	int		ex_status;
	int		failed;

	ex_status = MS_BLT_SUCC;
	while (tree != NULL)
	{
		if (blt_check_and_separate_env(master, tree->token, key_value))
			ex_status = MS_BLT_FAIL;
		else
		{
			failed = blt_check_and_export(tree, var, key_value);
			free(key_value[KEY]);
			free(key_value[VALUE]);
			if (failed)
				return (MS_BLT_FAIL);
		}
		tree = tree->right;
	}
	return (ex_status);
}

int	blt_join_env(t_shellvar *key_pos, char *key_value[2])
{
	char	*tmp;

	if (key_pos->value == NULL)
		key_pos->value = ft_strdup(key_value[VALUE]);
	else if (ft_strcmp(key_pos->value, "") == 0)
	{
		free(key_pos->value);
		key_pos->value = ft_strdup(key_value[VALUE]);
	}
	else
	{
		tmp = key_pos->value;
		key_pos->value = ft_strjoin(key_pos->value, key_value[VALUE]);
		free(tmp);
	}
	if (key_pos->value == NULL)
	{
		ms_perror("malloc");
		return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

/*
** $ export TEST=test aaa
** tree->token = "TEST=test"
** tree->right->token = "aaa"
*/
int	blt_export(t_shellvar *var, t_stree *tree, t_master *master)
{
	if (tree == NULL)
		blt_export_print_and_sort_env(var);
	else
	{
		if (blt_export_env(var, tree, master) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}
