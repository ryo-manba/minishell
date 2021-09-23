/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_restore_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/13 22:37:21 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/23 15:56:10 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

int	exec_get_env_size(t_shellvar *var)
{
	int			i;
	t_shellvar	*tmp;

	i = 0;
	tmp = var;
	while (tmp)
	{
		tmp = tmp->next;
		i += 1;
	}
	return (i);
}

char	*exec_restore(t_shellvar *var)
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
	int			sz;
	char		**env;
	int			i;
	t_shellvar	*tmp;

	i = -1;
	sz = exec_get_env_size(var);
	env = (char **)malloc(sizeof(char *) * (sz + 1));
	if (env == NULL)
		return (NULL);
	tmp = var;
	while (++i < sz)
	{
		env[i] = exec_restore(tmp);
		if (env[i] == NULL)
		{
			exec_all_free(env);
			return (NULL);
		}
		tmp = tmp->next;
	}
	env[i] = NULL;
	return (env);
}
