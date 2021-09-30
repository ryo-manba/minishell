/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_fx_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 22:41:12 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/30 22:41:23 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

int	ex_fx_list_is_for_fx(t_ex_token *ext)
{
	t_ex_token	*temp;

	temp = ext;
	while (temp && temp->token)
	{
		if (temp->token_id != XI_DQUOTED && temp->token_id != XI_SQUOTED)
		{
			if (!!ft_strchr((char *)temp->token, '*'))
				return (1);
		}
		temp = temp->right;
	}
	return (0);
}
