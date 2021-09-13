/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_restore_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/13 22:37:21 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/13 22:42:27 by rmatsuka         ###   ########.fr       */
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

char	**exec_restore(t_shellvar *var, char **env, int sz)
{
	int		i;
	char	*tmp;

	i = -1;
	while (++i < sz)
	{
		tmp = ft_strjoin(var->key, "=");
		if (tmp == NULL)
		{
			exec_all_free(env);
			return (NULL);
		}
		env[i] = ft_strjoin(tmp, var->value);
		if (env[i] == NULL)
		{
			free(tmp);
			exec_all_free(env);
			return (NULL);
		}
		free(tmp);
		var = var->next;
	}
	env[i] = NULL;
	return (env);
}

char	**exec_restore_env(t_shellvar *var)
{
	int			sz;
	char		**env;
	t_shellvar	*tmp;

	sz = exec_get_env_size(var);
	env = (char **)malloc(sizeof(char *) * sz + 1);
	if (env == NULL)
		return (NULL);
	tmp = var;
	return (exec_restore(tmp, env, sz));
}
