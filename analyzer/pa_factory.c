/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pa_factory.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:20:17 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/09 01:51:47 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

// factory funcs for parser.

t_stree	*pa_make_stree(t_wdlist *word, int for_subshell)
{
	t_stree	*st;

	st = (t_stree *)ft_calloc(1, sizeof(t_stree));
	if (!st)
		return (NULL);
	st->token = ft_substr(word->word, 0, word->len);
	if (!st->token)
	{
		free(st);
		return (NULL);
	}
	if (for_subshell)
		st->token_id = TI_SUBSHELL;
	else if (word->lex_type == LT_IO_NUMBER)
		st->token_id = TI_IO_NUMBER;
	else
		st->token_id = TI_WORD;
	return (st);
}

t_stree	*ex_make_stree(char *token, t_token_id tid)
{
	t_stree	*st;

	st = (t_stree *)ft_calloc(1, sizeof(t_stree));
	if (!st)
		return (NULL);
	st->token = token;
	st->token_id = tid;
	return (st);
}

t_redir	*pa_make_redir(t_wdlist *op_word, t_stree *target, t_stree *ion)
{
	t_redir		*redir;
	t_token_id	ti;

	ti = pa_operator_token_id(op_word);
	if (ti == TI_NONE)
		return (NULL);
	redir = (t_redir *)ft_calloc(1, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->operand_left = ion;
	redir->operand_right = target;
	redir->redir_op = ti;
	return (redir);
}
