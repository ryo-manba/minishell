/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_quoteutils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 19:51:17 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/22 21:47:03 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

int	ms_is_quoted(int bs[2], int i, char c, char quoted)
{
	if (bs[i % 2])
	{
		if (quoted == '\'')
			return (0);
		if (quoted == '"')
			return (!!ft_strchr("$`\"\\\n", c));
		return (!!ft_strchr("|&;<>()$`\\\"' \t\n", c));
	}
	return (0);
}

void	ms_update_bs(int bs[2], int i, char c, char quoted)
{
	if (c == '\\' && quoted != '\'')
		bs[(i + 1) % 2] = !bs[i % 2];
	else
		bs[(i + 1) % 2] = 0;
}

int	lx_is_quoted(t_lex_cursor *cursor, char c)
{
	return (ms_is_quoted(cursor->bs, cursor->i, c, cursor->under_quoted));
}

void	lx_update_bs(t_lex_cursor *cursor, char c)
{
	ms_update_bs(cursor->bs, cursor->i, c, cursor->under_quoted);
}
