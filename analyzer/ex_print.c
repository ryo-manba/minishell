/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 19:01:02 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/22 21:26:20 by yokawada         ###   ########.fr       */
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

void	ex_stringify_extoken_ifneeded(t_ex_token *ext, char *preemble)
{
	if (!PA_DEBUG)
		return ;
	printf("%s ", preemble);
	ex_stringify_extoken(ext);
}
