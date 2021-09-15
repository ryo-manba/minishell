/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:26:04 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/13 02:45:14 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

static int	init_parser(t_master *master, t_parse_state *state,
	t_wdlist *words, int for_subshell)
{
	ft_bzero(state, sizeof(t_parse_state));
	state->master = master;
	if (!pa_add_new_pipeline(state))
		return (pa_generic_error(state, NULL, "failed to alloc a pipeline"));
	state->cursor.word = words;
	state->for_subshell = for_subshell;
	return (MS_AZ_SUCC);
}

int	ms_parse(t_master *master, t_parse_state *state, t_wdlist *words,
		int for_subshell)
{
	if (init_parser(master, state, words, for_subshell))
		return (pa_generic_error(state, NULL, "failed to init parser"));
	while (1)
	{
		if (state->finished)
			break ;
		if (state->err_message)
			break ;
		if (pa_unit(state))
			break ;
	}
	if (state->failed)
	{
		pa_destroy_pipeline(state->pipeline);
		state->pipeline = NULL;
		return (MS_AZ_FAIL);
	}
	return (MS_AZ_SUCC);
}
