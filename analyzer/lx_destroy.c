/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_destroy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 23:47:20 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/05 23:47:48 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

void	lx_destroy_token(t_wdlist *t)
{
	if (!t)
		return ;
	lx_destroy_token(t->next);
	free(t);
}
