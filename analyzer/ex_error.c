/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 19:01:30 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/05 19:01:30 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_expander.h"

void	*ex_error(t_ex_state *state, t_stree *stree, char *message)
{
	(void)state;
	(void)stree;
	ft_putstr_fd(message, STDERR_FILENO);
	return (NULL);
}

void	*ex_fatal(t_ex_state *state, char *message)
{
	(void)state;
	ft_putstr_fd(message, STDERR_FILENO);
	return (NULL);
}
