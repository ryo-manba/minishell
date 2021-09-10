/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_check.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 17:52:58 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/10 05:01:25 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

// 引数が正しいかチェックして key と value で分ける
int	blt_check_and_separate_env(char *token, char *key_value[2])
{
	int32_t	equal_idx;

	equal_idx = ft_strchr_i(token, '=');
	if (blt_check_export_key(token, key_value, equal_idx) == MS_BLT_FAIL)
	{
		blt_export_print_error(token);
		return (MS_BLT_FAIL);
	}
	if (blt_separate_key_value(equal_idx, token, key_value) == MS_BLT_FAIL)
	{
		ms_print_perror("malloc");
		return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

// export の key が形式に沿っているかチェックする
// '='がない場合文字を終端まで見る
int	blt_check_export_key(char *token, char *key_value[2], int32_t equal_idx)
{
	int32_t	i;

	if (equal_idx == -1)
		equal_idx = ft_strlen(token);
	i = 0;
	while (i < equal_idx)
	{
		if ((i == 0 && ft_isalpha(token[i] == 0) && token[i] != '_')
			|| (ft_isalnum(token[i]) != 1 && token[i] != '_'))
			return (MS_BLT_FAIL);
		i += 1;
	}
	return (MS_BLT_SUCC);
}

void	blt_export_print_error(char *token)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier\n", STDERR_FILENO);
}

// '+='と'='で分ける
// valueがない場合はNULL埋め
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