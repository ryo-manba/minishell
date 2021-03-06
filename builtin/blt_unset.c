/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 18:00:18 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/27 10:46:19 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

static void	blt_unset_head(t_shellvar **env)
{
	t_shellvar	*tmp;

	tmp = *env;
	*env = tmp->next;
	ms_env_free(tmp);
}

static void	blt_unset_second_and_subsequent(
		t_shellvar *env, t_shellvar *key_pos)
{
	t_shellvar	*head;
	t_shellvar	*tmp;

	head = env;
	while (head->next != key_pos)
		head = head->next;
	tmp = head->next;
	head->next = head->next->next;
	ms_env_free(tmp);
}

static int	blt_check_identifier(t_master *master, t_stree *tree)
{
	if (lx_str_is_for_name(tree->token, -1))
		return (MS_BLT_SUCC);
	exec_error_prologue(master, 0);
	ft_putstr_fd("unset: `", STDERR_FILENO);
	ft_putstr_fd(tree->token, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (MS_BLT_FAIL);
}

/**
 * $ unset PWD USER
 * tree->token = PWD
 * tree->right->token = USER
 */
int	blt_unset(t_master *master, t_stree *tree)
{
	t_shellvar	**env;
	t_shellvar	*key_pos;
	int			rv;

	rv = MS_BLT_SUCC;
	env = &(master->var);
	while (tree != NULL)
	{
		if (blt_check_identifier(master, tree))
			rv = MS_BLT_FAIL;
		else
		{
			key_pos = ms_search_key(*env, tree->token);
			if (key_pos != NULL)
			{
				if (*env == key_pos)
					blt_unset_head(env);
				else
					blt_unset_second_and_subsequent(*env, key_pos);
			}
		}
		tree = tree->right;
	}
	return (rv);
}
