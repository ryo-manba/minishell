/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_cursor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 19:01:57 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/27 01:45:51 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

void	ex_add_token_csr(t_ex_part_cursor *cursor, t_ex_token *ext)
{
	if (cursor->tail)
		cursor->tail->right = ext;
	cursor->tail = ext;
	if (!cursor->head)
		cursor->head = cursor->tail;
}

void	ex_ll_init_cursor(t_ex_unit_cursor *cursor, t_token_id tid,
	const char *str, char quote)
{
	ft_bzero(cursor, sizeof(t_ex_unit_cursor));
	cursor->str = str;
	cursor->quote = quote;
	cursor->running = XI_NEUTRAL;
	cursor->pa_token_id = tid;
	cursor->n = !!str[0] + ft_strlen_upto_c(str + !!str[0], quote);
	cursor->vs = !!quote;
	cursor->i = cursor->vs;
}

void	ex_init_cursor_mid(t_ex_unit_cursor *cursor, t_ex_token *ext)
{
	ft_bzero(cursor, sizeof(t_ex_unit_cursor));
	cursor->s.head = ext;
	cursor->s.tail = ext;
}
