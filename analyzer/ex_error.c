/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 19:01:30 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/06 22:16:15 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

void	*ex_error(t_ex_state *state, t_stree *stree, char *message)
{
	if (!state->error_printed && stree)
	{
		ft_putstr_fd(MS_AZ_PROGNAME ": ", STDERR_FILENO);
		ft_putstr_fd(stree->token, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		state->error_printed = 1;
	}
	free(stree);
	ex_mark_failed(state, 1, message);
	return (NULL);
}
