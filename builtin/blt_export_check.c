/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_check.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 17:52:58 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/23 20:43:34 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

int	blt_check_and_separate_env(
		t_master *master, char *token, char *key_value[2])
{
	int32_t	equal_idx;

	equal_idx = ft_strchr_i(token, '=');
	if (blt_check_export_key(token, equal_idx) == MS_BLT_FAIL)
	{
		blt_export_print_error(master, token);
		return (MS_BLT_FAIL);
	}
	if (blt_separate_key_value(equal_idx, token, key_value) == MS_BLT_FAIL)
	{
		ms_perror("malloc");
		return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

// Check the export key follows the format
int	blt_check_export_key(char *token, int32_t equal_idx)
{
	int32_t	i;

	i = 0;
	if (equal_idx == 0)
		return (MS_BLT_FAIL);
	if (equal_idx == -1)
		equal_idx = ft_strlen(token);
	else if (equal_idx >= 1 && token[equal_idx - 1] == '+')
		equal_idx -= 1;
	while (i < equal_idx)
	{
		if ((i == 0 && (ft_isalpha(token[i]) || token[i] == '_')))
			i += 1;
		else if (i >= 1 && (ft_isalpha(token[i]) || \
			ft_isalnum(token[i]) || token[i] == '_'))
			i += 1;
		else
			return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

void	blt_export_print_error(t_master *master, char *token)
{
	exec_error_prologue(master, 0);
	ft_putstr_fd("export: `", STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

int	blt_separate_key_value(int32_t equal_idx, char *token, char *key_value[2])
{
	if (equal_idx != -1 && token[equal_idx - 1] == '+')
		key_value[KEY] = ft_substr(token, 0, equal_idx - 1);
	else
		key_value[KEY] = ft_substr(token, 0, equal_idx);
	if (key_value[KEY] == NULL)
		return (MS_BLT_FAIL);
	if (equal_idx == -1)
		key_value[VALUE] = NULL;
	else
	{
		key_value[VALUE] = ft_substr(token, equal_idx + 1, ft_strlen(token));
		if (key_value[VALUE] == NULL)
		{
			free(key_value[KEY]);
			return (MS_BLT_FAIL);
		}
	}
	return (MS_BLT_SUCC);
}
