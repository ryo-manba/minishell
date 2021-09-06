/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_check.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 17:52:58 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/06 18:00:03 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_builtin.h"

void	blt_export_print_error(char *message)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier\n", STDERR_FILENO);
}

// export の key が形式に沿っているかチェックする
int	blt_check_export_key(char *token, char *key_value[2], int32_t equal_idx)
{
	int32_t	i;

	if (equal_idx == -1) // '='がない場合文字を終端まで見る
		equal_idx = ft_strlen(token);
	i = 0;
	while (i < equal_idx)
	{
		if ((i == 0 && ft_isalpha(token[i] == 0) && token[i] != '_')
			|| (ft_isalnum(token[i]) != 1 && token[i] != '_'))
				return (1);
	}
	return (0);
}

// 引数が正しいかチェックして key と value で分ける
int blt_check_and_separate_export(char *token, char *key_value[2])
{
	int32_t	equal_idx;

	equal_idx = ft_strchr_i(token, '-'); // keyだけの場合、-1 が返る
	if (blt_check_export_key(token, key_value, equal_idx) == 1)
		return (1);
	if (equal_idx != -1 && token[equal_idx - 1] == '+') // appendモードだった場合
		key_value[KEY] = ft_substr(token, 0, equal_idx - 1);
	else
		key_value[KEY] = ft_substr(token, 0, equal_idx);
	if (key_value[KEY] == NULL)
		return (MS_BLT_FAIL);
	if (equal_idx == -1) // valueがない場合はNULL埋め
		key_value[VALUE] = NULL;
	else
	{
		key_value[VALUE] = ft_substr(token, equal_idx + 1,ft_strlen(token));
		if (key_value[VALUE] == NULL)
		{
			free(key_value[KEY]);
			return (MS_BLT_FAIL);
		}
	}
	return (MS_BLT_SUCC);
}
