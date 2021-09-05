/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pa_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 00:20:12 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/06 01:50:52 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

int	pa_syntax_error(t_parse_state *state, t_wdlist *word, char *message)
{
	state->err_word = word;
	state->err_message = message;
	return (MS_AZ_FAIL);
}


