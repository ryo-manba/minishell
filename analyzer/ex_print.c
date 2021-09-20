/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 19:01:02 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/20 11:49:34 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

void	ex_stringify_extoken(t_ex_token *ext)
{
	while (ext)
	{
		if (ext->token_id == XI_DIVIDER)
			printf("[DIV] ");
		else
		{
			printf("[%d {%s} (%d)] ",
				ext->token_id, ext->token, ext->pa_token_id);
		}
		ext = ext->right;
	}
	printf("\n");
}
