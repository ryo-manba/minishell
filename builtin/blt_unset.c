/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 18:00:18 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/19 11:33:16 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

// unsetで指定された要素が先頭だった場合
void	blt_unset_head(t_shellvar **env)
{
	t_shellvar	*tmp;

	tmp = *env;
	*env = tmp->next;
	ms_env_free(tmp);
}

// "unset b"  a->b->c を a->c free(b)
void	blt_unset_second_and_subsequent(t_shellvar *env, t_shellvar *key_pos)
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

/**
 * $ unset PWD USER
 * tree->token = PWD
 * tree->right->token = USER
 */
int	blt_unset(t_master *master, t_stree *tree)
{
	t_shellvar	**env;
	t_shellvar	*key_pos;

	env = &(master->var);
	while (tree != NULL)
	{
		key_pos = ms_search_key(*env, tree->token);
		if (key_pos != NULL)
		{
			if (*env == key_pos)
				blt_unset_head(env);
			else
				blt_unset_second_and_subsequent(*env, key_pos);
		}
		tree = tree->right;
	}
	return (MS_BLT_SUCC);
}
