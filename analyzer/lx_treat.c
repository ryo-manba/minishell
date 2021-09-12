/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_treat.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:31:20 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/12 17:53:17 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

int	lx_treat_quote(t_lex_cursor *cursor, char c, char ct)
{
	if (c == cursor->under_quoted)
		cursor->under_quoted = '\0';
	else if (!cursor->under_quoted
		&& (ct == LC_SINGLE_QUOTE || ct == LC_DOUBLE_QUOTE))
	{
		cursor->under_quoted = ct;
		if (lx_add_token(cursor, ct))
			return (lx_mark_failed(cursor, LX_ERR_GEN, "add l-token(quote)"));
	}
	else if (cursor->under_quoted || ct == LC_WORD)
	{
		if (cursor->tail
			&& !ft_strchr(CHARS_WORD_INCLUDED, cursor->tail->starting_chartype))
			lx_conclude_token(cursor);
		if (lx_add_token(cursor, ct))
			return (lx_mark_failed(cursor, LX_ERR_GEN, "add l-token(unquote)"));
	}
	else
		return (0);
	cursor->i += 1;
	return (1);
}

int	lx_treat_brace(t_lex_cursor *cursor, char c, char ct)
{
	if (cursor->under_brace && c == LC_BRACE_R)
		cursor->under_brace = 0;
	else if (!cursor->under_brace
		&& ct == LC_BRACE_L)
	{
		cursor->under_brace = 1;
		if (lx_add_token(cursor, ct))
			return (lx_mark_failed(cursor, LX_ERR_GEN, "add l-token(brace)"));
	}
	else if (cursor->under_brace || ct == LC_WORD)
	{
		if (cursor->tail
			&& !ft_strchr(CHARS_WORD_INCLUDED, cursor->tail->starting_chartype))
			lx_conclude_token(cursor);
		if (lx_add_token(cursor, ct))
			return (lx_mark_failed(cursor, LX_ERR_GEN, "add l-token(unbrace)"));
	}
	else
		return (0);
	cursor->i += 1;
	return (1);
}

int	lx_treat_nl(t_lex_cursor *cursor, char c, char ct)
{
	(void)c;
	if (ct != LC_NEWLINE)
		return (0);
	lx_conclude_token(cursor);
	if (lx_add_token(cursor, ct))
		return (lx_mark_failed(cursor, LX_ERR_GEN, "add l-token(nl)"));
	cursor->i += 1;
	return (1);
}

int	lx_treat_space(t_lex_cursor *cursor, char c, char ct)
{
	(void)c;
	if (ct != LC_SPACE)
		return (0);
	lx_conclude_token(cursor);
	cursor->i += 1;
	return (1);
}

int	lx_treat_operator(t_lex_cursor *cursor, char c, char ct)
{
	if (!ft_strchr(LX_OPERATOR_OPENER, c))
		return (0);
	if (!cursor->tail || cursor->tail->concluded)
		if (lx_add_token(cursor, ct))
			return (lx_mark_failed(cursor, LX_ERR_GEN, "add l-token(op)"));
	if (!ft_strchr(LX_OPERATOR_OPENER, cursor->line[cursor->tail->i]))
	{
		lx_conclude_token(cursor);
		if (lx_add_token(cursor, ct))
			return (lx_mark_failed(cursor, LX_ERR_GEN, "a lex-token(op)"));
	}
	cursor->i += lx_cut_operator(cursor);
	lx_conclude_token(cursor);
	return (1);
}
