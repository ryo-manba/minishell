/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_fx_cwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/04 23:34:11 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/23 02:03:20 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

static t_ex_token	*ex_fx_push_back_token(t_ex_state *state,
		t_ex_part_cursor *csr, char *str)
{
	t_ex_token	*ext;
	char		*cloned_str;

	ext = (t_ex_token *)ft_calloc(1, sizeof(t_ex_token));
	cloned_str = ft_strdup(str);
	if (!ext || !cloned_str)
	{
		free(ext);
		free(cloned_str);
		ex_mark_failed(state, 1, "[FX] alloc ex-token");
		return (NULL);
	}
	ext->token = cloned_str;
	ext->n = ft_strlen(ext->token);
	if (csr->tail)
		csr->tail->right = ext;
	csr->tail = ext;
	if (!csr->head)
		csr->head = csr->tail;
	return (ext);
}

static void	ex_fx_sort_strarr(t_ex_part_cursor *csr, char **strarr, size_t n)
{
	char	*temp;
	size_t	i;
	size_t	j;

	i = 0;
	csr->tail = csr->head;
	while (i < n)
	{
		j = n;
		while (i < --j)
		{
			if (ft_strcmp(strarr[j - 1], strarr[j]) > 0)
			{
				temp = strarr[j];
				strarr[j] = strarr[j - 1];
				strarr[j - 1] = temp;
			}
		}
		csr->tail->token = strarr[i++];
		csr->tail->n = ft_strlen(csr->tail->token);
		csr->tail = csr->tail->right;
	}
}

static void	ex_fx_sort_dir_ents(t_ex_state *state,
		t_ex_part_cursor *csr, size_t n)
{
	char	**strarr;
	size_t	i;

	strarr = (char **)malloc(n * sizeof(char *));
	if (!strarr)
	{
		ex_mark_failed(state, 1, "[FX] alloc dir ent");
		return ;
	}
	csr->tail = csr->head;
	i = 0;
	while (i < n)
	{
		strarr[i++] = (char *)csr->tail->token;
		csr->tail = csr->tail->right;
	}
	ex_fx_sort_strarr(csr, strarr, n);
	free(strarr);
}

static size_t	ex_open_and_pack(t_ex_state *state, t_ex_part_cursor *csr)
{
	char			*str;
	DIR				*dir;
	struct dirent	*dir_ent;
	size_t			n;

	str = getcwd(NULL, 0);
	dir = opendir(str);
	free(str);
	if (!dir)
		return (0);
	n = 0;
	while (!state->failed)
	{
		dir_ent = readdir(dir);
		if (!dir_ent)
			break ;
		ex_fx_push_back_token(state, csr, dir_ent->d_name);
		n += 1;
	}
	closedir(dir);
	state->failed = state->failed || !!errno;
	return (n);
}

t_ex_token	*ex_fx_dir_ents(t_ex_state *state)
{
	t_ex_part_cursor	csr;
	size_t				n;

	errno = 0;
	ft_bzero(&csr, sizeof(csr));
	n = ex_open_and_pack(state, &csr);
	if (!state->failed)
		ex_fx_sort_dir_ents(state, &csr, n);
	if (state->failed)
	{
		ex_destroy_token(csr.head);
		csr.head = NULL;
	}
	state->file_names = csr.head;
	return (csr.head);
}
