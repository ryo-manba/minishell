/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_fx_match.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 18:59:33 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/05 21:49:38 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

int	ex_fx_exec_match(t_ex_fx_dpcursor *dc, t_ex_token *file)
{
	size_t	i;
	size_t	j;

	j = 0;
	ft_bzero(dc->dp[j % 2], (dc->n + 1) * sizeof(unsigned char));
	dc->dp[j % 2][0] = 1;
	while (j <= file->n)
	{
		i = 0;
		while (i <= dc->n)
		{
			if (0 < j && dc->pattern[i] == '*')
				dc->dp[j % 2][i] = dc->dp[!(j % 2)][i];
			if (0 < i && dc->pattern[i - 1] == '*')
				dc->dp[j % 2][i] = dc->dp[j % 2][i] || dc->dp[j % 2][i - 1];
			if (0 < i && 0 < j
				&& (dc->pattern[i - 1] == '*'
					|| dc->pattern[i - 1] == file->token[j - 1]))
				dc->dp[j % 2][i] = dc->dp[j % 2][i] || dc->dp[!(j % 2)][i - 1];
			i += 1;
		}
		j += 1;
		ft_bzero(dc->dp[j % 2], (dc->n + 1) * sizeof(unsigned char));
	}
	return (!!dc->dp[(file->n % 2)][dc->n]);
}

void	ex_fx_match_loop(t_ex_state *state, t_ex_unit_cursor *cursor,
		t_ex_fx_dpcursor *dpcursor)
{
	t_ex_token		*file;

	dpcursor->match_hidden = dpcursor->pattern[0] == '.';
	file = state->file_names;
	while (!state->failed && file)
	{
		if ((dpcursor->match_hidden || file->token[0] != '.')
			&& ex_fx_exec_match(dpcursor, file))
		{
			cursor->vs = 0;
			cursor->i = file->n;
			file->token_id = cursor->running;
			file->pa_token_id = cursor->pa_token_id;
			ex_clone_and_push_back_token(state, cursor, file);
			if (!state->failed)
				ex_push_back_divider_if_needed(state, cursor, cursor->p.tail);
			dpcursor->mathched += 1;
		}
		file = file->right;
	}
}

size_t	ex_fx_expand(t_ex_state *state, t_ex_unit_cursor *cursor,
	char *pattern, size_t n)
{
	t_ex_fx_dpcursor	dpcsr;

	if (!state->file_names)
		state->file_names = ex_fx_dir_ents(state);
	if (state->failed)
		return (0);
	ft_bzero(&dpcsr, sizeof(t_ex_fx_dpcursor));
	dpcsr.pattern = pattern;
	dpcsr.n = n;
	dpcsr.dp[0] = (unsigned char *)ft_calloc((n + 1), sizeof(unsigned char));
	dpcsr.dp[1] = (unsigned char *)ft_calloc((n + 1), sizeof(unsigned char));
	if (!dpcsr.dp[0] || !dpcsr.dp[1])
		ex_mark_failed(state, 1, "[FX] init ex-fx-dp");
	else
		ex_fx_match_loop(state, cursor, &dpcsr);
	free(dpcsr.dp[0]);
	free(dpcsr.dp[1]);
	return (dpcsr.mathched);
}
