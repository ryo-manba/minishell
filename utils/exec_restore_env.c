/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_restore_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/13 22:37:21 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/30 20:26:55 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

static size_t	exec_get_env_size(t_shellvar *var)
{
	size_t		i;
	t_shellvar	*tmp;

	i = 0;
	tmp = var;
	while (tmp)
	{
		if (tmp->is_env)
			i += 1;
		tmp = tmp->next;
	}
	return (i);
}

static char	*exec_restore(t_shellvar *var)
{
	char	*tmp;
	char	*env;

	if (var->value == NULL)
		env = ft_strdup(var->key);
	else
	{
		tmp = ft_strjoin(var->key, "=");
		if (tmp == NULL)
			return (NULL);
		env = ft_strjoin(tmp, var->value);
		free(tmp);
	}
	if (env == NULL)
		return (NULL);
	return (env);
}

char	**exec_restore_env(t_shellvar *var)
{
	size_t		i;
	size_t		sz;
	char		**env;
	t_shellvar	*tmp;

	i = 0;
	sz = exec_get_env_size(var);
	env = (char **)ft_calloc(sz + 1, sizeof(char *));
	if (env == NULL)
		return (NULL);
	tmp = var;
	while (tmp)
	{
		if (tmp->is_env)
		{
			env[i] = exec_restore(tmp);
			if (env[i++] == NULL)
			{
				exec_all_free(env);
				return (NULL);
			}
		}
		tmp = tmp->next;
	}
	return (env);
}
