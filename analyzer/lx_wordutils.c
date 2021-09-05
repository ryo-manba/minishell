/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_wordutils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:30:53 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/06 02:17:13 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

static t_wdlist	*new_lexer_token(int i, char starting_char)
{
	t_wdlist	*new;

	new = (t_wdlist *)ft_calloc(1, sizeof(t_wdlist));
	if (new)
	{
		new->i = i;
		new->starting_chartype = starting_char;
		new->lex_type = LT_TOKEN;
	}
	return (new);
}

// returns 1 if it failed
int	lx_add_token(t_lex_cursor *cursor, char ct)
{
	t_wdlist	*new;

	if (cursor->tail && !(cursor->tail->concluded))
		return (MS_AZ_SUCC);
	new = new_lexer_token(cursor->i, ct);
	if (!new)
		return (lx_mark_failed(cursor, 1, "alloc lex-token"));
	if (cursor->tail)
		cursor->tail->next = new;
	cursor->tail = new;
	if (!cursor->head)
		cursor->head = cursor->tail;
	cursor->tail->word = cursor->line + cursor->tail->i;
	return (MS_AZ_SUCC);
}

// always succeeds
void	lx_conclude_token(t_lex_cursor *cursor)
{
	t_wdlist	*tail;
	char		delimiter;

	tail = cursor->tail;
	if (!tail || tail->concluded)
		return ;
	delimiter = cursor->line[cursor->i];
	tail->delimiter = delimiter;
	tail->len = cursor->i - tail->i;
	tail->concluded = 1;
	if (tail->starting_chartype == LC_NEWLINE)
		tail->lex_type = LT_NEWLINE;
	else if (lx_is_an_operator(cursor))
		tail->lex_type = LT_OPERATOR;
	else if (ft_strchr("<>", tail->delimiter)
		&& lx_str_is_digital(tail->word, tail->len))
		cursor->tail->lex_type = LT_IO_NUMBER;
}
