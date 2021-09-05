/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_operator.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:26:24 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/06 00:27:47 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

const char			*g_op_label[] = {
	"<<-",
	"<<",
	">>",
	"<&",
	">&",
	"<>",
	"&&",
	"||",
	"<",
	">",
	"|",
	"&",
	";",
	"(",
	")",
	NULL};

const t_token_id	g_op_token_id[] = {
	TI_LTLTHYPHEN,
	TI_LTLT,
	TI_GTGT,
	TI_LTAND,
	TI_GTAND,
	TI_LTGT,
	TI_ANDAND,
	TI_PIPEPIPE,
	TI_LT,
	TI_GT,
	TI_PIPE,
	TI_AND,
	TI_SEMICOLON,
	TI_PAREN_L,
	TI_PAREN_R,
	0};

const size_t		g_op_len[] = {
	3,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	0};

const t_token_id	g_all_token_id[] = {
	TI_WORD,
	TI_IO_NUMBER,
	TI_NAME,
	TI_ASSIGNMENT_WORD,
	TI_SUBSHELL,
	// [redirection]
	TI_LT, // <
	TI_GT, // >
	TI_LTLT, // <<
	TI_GTGT, // >>
	TI_LTGT, // <>
	TI_LTAND, // <&
	TI_GTAND, // >&
	TI_LTLTHYPHEN, // <<-
	// [term clause]
	TI_PIPE, // |
	// [term pipeline]
	TI_ANDAND, // &&
	TI_PIPEPIPE, // ||
	TI_AND, // &
	TI_SEMICOLON, // ;
	// [open and close subshell]
	TI_PAREN_L, // (
	TI_PAREN_R, // )
	TI_NONE,
	0};

const char			*g_all_token_label[] = {
	"WORD",
	"IO_NUMBER",
	"NAME",
	"ASSIGNMENT_WORD",
	"SUBSHELL",
	// [redirection]
	"<",
	">",
	"<<",
	">>",
	"<>",
	"<&",
	">&",
	"<<-",
	// [term clause]
	"|",
	// [term pipeline]
	"&&",
	"||",
	"&",
	";",
	// [open and close subshell]
	"(",
	")",
	"NONE",
	NULL};

// returns 1 if active t_wdlist is an operator.
int	lx_is_an_operator(t_lex_cursor *cursor)
{
	const char	*strhead;
	int			i;
	size_t		d;

	if (!cursor || !cursor->tail)
		return (0);
	strhead = cursor->line + cursor->tail->i;
	i = -1;
	while (g_op_label[++i])
	{
		d = cursor->i - cursor->tail->i;
		if (d == g_op_len[i]
			&& !ft_strncmp(strhead, g_op_label[i], g_op_len[i]))
			return (1);
	}
	return (0);
}

// `line[i]`から最も長い演算子が取れるまで`i`を進める
size_t	lx_cut_operator(t_lex_cursor *cursor)
{
	size_t	k;
	size_t	n;

	k = 0;
	while (g_op_label[k])
	{
		n = ft_starts_with(&(cursor->line[cursor->i]), g_op_label[k]);
		if (n)
			return (n);
		k += 1;
	}
	return (0);
}

// returns a t_token_id for given t_wdlist(only for operator)
t_token_id	pa_operator_token_id(t_wdlist *word)
{
	int	i;

	i = -1;
	while (g_op_label[++i])
	{
		if (!ft_strncmp(word->word, g_op_label[i], g_op_len[i]))
			return (g_op_token_id[i]);
	}
	return (TI_NONE);
}

// returns a label string for given t_token_id(only for operator)
const char	*pa_operator_label(t_token_id ti)
{
	int	i;

	i = -1;
	while (g_op_token_id[++i])
	{
		if (g_op_token_id[i] == ti)
			return (g_op_label[i]);
	}
	return (NULL);
}

// returns a label string for given t_token_id
const char	*pa_token_label(t_token_id ti)
{
	int	i;

	i = -1;
	while (g_all_token_id[++i])
	{
		if (g_all_token_id[i] == ti)
			return (g_all_token_label[i]);
	}
	return (NULL);
}
