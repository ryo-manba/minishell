/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_operator.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:26:24 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/27 09:28:50 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

// returns 1 if active t_wdlist is an operator.
int	lx_tail_is_an_operator(t_lex_cursor *cursor)
{
	const char	*strhead;
	int			i;
	size_t		d;
	t_op		*ops;

	if (!cursor || !cursor->tail)
		return (0);
	ops = cursor->master->lx_ops;
	strhead = cursor->line + cursor->tail->i;
	i = -1;
	while (ops[++i].label)
	{
		d = cursor->i - cursor->tail->i;
		if (d == ops[i].len
			&& !ft_strncmp(strhead, ops[i].label, ops[i].len))
			return (1);
	}
	return (0);
}

size_t	lx_cut_operator(t_lex_cursor *cursor)
{
	size_t	k;
	size_t	n;
	t_op	*ops;

	k = 0;
	ops = cursor->master->lx_ops;
	while (ops[k].label)
	{
		n = ft_starts_with(&(cursor->line[cursor->i]), ops[k].label);
		if (n)
			return (n);
		k += 1;
	}
	return (0);
}

// returns a t_token_id for given t_wdlist(only for operator)
t_token_id	pa_operator_token_id(t_parse_state *state, t_wdlist *word)
{
	int		i;
	t_op	*ops;

	ops = state->master->lx_ops;
	i = -1;
	while (ops[++i].label)
	{
		if (!ft_strncmp(word->word, ops[i].label, ops[i].len))
			return (ops[i].token_id);
	}
	return (TI_NONE);
}

// returns a label string for given t_token_id(only for operator)
const char	*pa_operator_label(t_master *master, t_token_id ti)
{
	int		i;
	t_op	*ops;

	ops = master->lx_ops;
	i = -1;
	while (ops[++i].label)
	{
		if (ops[i].token_id == ti)
			return (ops[i].label);
	}
	return (NULL);
}

// returns a label string for given t_token_id
const char	*pa_token_label(t_master *master, t_token_id ti)
{
	int		i;
	t_op	*ops;

	ops = master->lx_all_ops;
	i = -1;
	while (ops[++i].label)
	{
		if (ops[i].token_id == ti)
			return (ops[i].label);
	}
	return (NULL);
}
