/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pa_redir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:21:37 by yokawada          #+#    #+#             */
/*   Updated: 2021/10/01 20:53:02 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

static int	pa_syntax_error_wrapper(t_stree *ion_st, t_parse_state *state,
	t_wdlist *word, char *message)
{
	pa_destroy_stree(ion_st);
	return (pa_syntax_error(state, word, message));
}

int	pa_redirection(t_parse_state *state, t_wdlist *word, t_stree *ion_st)
{
	t_wdlist	*target_wd;
	t_stree		*target_st;
	t_redir		*redir;

	target_wd = pa_shift_lx_token(state);
	if (!target_wd)
		return (pa_syntax_error_wrapper(ion_st,
				state, word, "NO_RIGHT_OPERAND"));
	if (target_wd->lex_type != LT_TOKEN)
		return (pa_syntax_error_wrapper(ion_st,
				state, target_wd, "NO_RIGHT_OPERAND"));
	target_st = pa_make_stree(target_wd, 0);
	if (!target_st)
		return (pa_syntax_error_wrapper(ion_st,
				state, target_wd, "ALLOCATION FAILED"));
	redir = pa_make_redir(state, word, target_st, ion_st);
	if (!redir)
		return (pa_syntax_error_wrapper(ion_st,
				state, word, "ALLOCATION FAILED"));
	return (!pa_add_redir(state, redir));
}
