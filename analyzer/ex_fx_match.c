/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_fx_match.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 18:59:33 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/30 22:10:57 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

static int	ex_fx_exec_match(t_ex_fx_dpcursor *dc, t_ex_token *file)
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

int	ex_fx_match_mark_step(t_ex_fx_dpcursor *dc, char pat[2], int qat[2])
{
	if (!dc->pattern_head || !dc->pattern_head->token)
		return (0);
	if (!dc->pattern_head->token[dc->i])
	{
		dc->i = 0;
		dc->pattern_head = dc->pattern_head->right;
	}
	pat[0] = pat[1];
	qat[0] = qat[1];
	if (!dc->pattern_head || !dc->pattern_head->token)
	{
		pat[1] = '\0';
		qat[1] = 0;
		return (0);
	}
	pat[1] = dc->pattern_head->token[dc->i];
	qat[1] = dc->pattern_head->token_id != XI_DQUOTED && dc->pattern_head->token_id != XI_SQUOTED;
	return (1);
}

int	ex_fx_exec_match2(t_ex_fx_dpcursor *dc, t_ex_token *file)
{
	size_t	i;
	size_t	j;
	char	pat[2];
	int		qat[2];

	j = 0;
	ft_bzero(dc->dp[j % 2], (dc->n + 1) * sizeof(unsigned char));
	dc->dp[j % 2][0] = 1;
	while (j <= file->n)
	{
		ft_bzero(pat, 2 * sizeof(unsigned char));
		ft_bzero(qat, 2 * sizeof(unsigned char));
		dc->pattern_head = dc->pattern_list;
		dc->i = 0;
		i = 0;
		while (i <= dc->n)
		{
			ex_fx_match_mark_step(dc, pat, qat);
			if (0 < j && pat[1] == '*' && qat[1])
				dc->dp[j % 2][i] = dc->dp[!(j % 2)][i];
			if (0 < i && pat[0] == '*' && qat[0])
				dc->dp[j % 2][i] = dc->dp[j % 2][i] || dc->dp[j % 2][i - 1];
			if (0 < i && 0 < j
				&& ((pat[0] == '*'  && qat[0])
					|| pat[0] == file->token[j - 1]))
				dc->dp[j % 2][i] = dc->dp[j % 2][i] || dc->dp[!(j % 2)][i - 1];
			i += 1;
			dc->i += 1;
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

void	ex_fx_match_loop2(t_ex_state *state, t_ex_unit_cursor *cursor,
		t_ex_fx_dpcursor *dpcursor)
{
	t_ex_token		*file;

	while (dpcursor->pattern_list && dpcursor->pattern_list->token
		&& ft_strlen(dpcursor->pattern_list->token) == 0)
		dpcursor->pattern_list = dpcursor->pattern_list->right;
	if (!dpcursor->pattern_list)
		return ;
	dpcursor->match_hidden = dpcursor->pattern_list->token[0] == '.';
	file = state->file_names;
	while (!state->failed && file)
	{
		if ((dpcursor->match_hidden || file->token[0] != '.')
			&& ex_fx_exec_match2(dpcursor, file))
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
	ex_destroy_token(state->file_names);
	state->file_names = NULL;
	return (dpcsr.mathched);
}

size_t	ex_fx_expand_lst(t_ex_state *state, t_ex_unit_cursor *cursor,
	t_ex_token *pattern, size_t n)
{
	t_ex_fx_dpcursor	dpcsr;

	if (!state->file_names)
		state->file_names = ex_fx_dir_ents(state);
	if (state->failed)
		return (0);
	ft_bzero(&dpcsr, sizeof(t_ex_fx_dpcursor));
	dpcsr.pattern_list = pattern;
	dpcsr.n = n;
	dpcsr.dp[0] = (unsigned char *)ft_calloc((n + 1), sizeof(unsigned char));
	dpcsr.dp[1] = (unsigned char *)ft_calloc((n + 1), sizeof(unsigned char));
	if (!dpcsr.dp[0] || !dpcsr.dp[1])
		ex_mark_failed(state, 1, "[FX] init ex-fx-dp");
	else
		ex_fx_match_loop2(state, cursor, &dpcsr);
	free(dpcsr.dp[0]);
	free(dpcsr.dp[1]);
	ex_destroy_token(state->file_names);
	state->file_names = NULL;
	return (dpcsr.mathched);
}
