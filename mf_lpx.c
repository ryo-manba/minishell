/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mf_lpx.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/13 03:52:35 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/27 11:12:29 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_pipeline	*mf_lex_parse(t_master *master,
	char *line, t_parse_state *ps)
{
	t_lex_cursor	lx_cursor;
	t_wdlist		*lexer_token;

	lexer_token = ms_lexer(master, line, &lx_cursor);
	if (!lexer_token || lx_cursor.failed)
	{
		if (lx_cursor.failed)
			g_ex_states = lx_cursor.failed;
		lx_destroy_token(lexer_token);
		return (NULL);
	}
	if (MS_DEBUG & MS_DEBUG_PRINT_PARSED)
		print_words(lexer_token);
	ms_parse(master, ps, lexer_token, 0);
	lx_destroy_token(lexer_token);
	if (ps->failed)
	{
		pa_destroy_pipeline(ps->pipeline);
		g_ex_states = ps->failed;
		ps->pipeline = NULL;
	}
	if (MS_DEBUG & MS_DEBUG_PRINT_PARSED)
		print_pipeline(master, ps->pipeline, 0);
	return (ps->pipeline);
}

t_pipeline	*mf_parse(t_master *master, char *line, t_parse_state *ps)
{
	if (!line || ft_strlen(line) == 0)
		return (NULL);
	master->current_line = line;
	if (master->interactive_shell)
		add_history(line);
	mf_lex_parse(master, line, ps);
	return (ps->pipeline);
}
