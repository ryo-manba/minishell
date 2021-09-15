/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_ll_spvar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 16:27:40 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/15 09:45:07 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

int	ex_ll_now_at_special_var(t_ex_unit_cursor *csr, size_t pos)
{
	const char	c = csr->str[pos];
	if (PA_DEBUG)
		printf("s is %zu, c is %c\n", csr->vs + 1, c);
	return (c && ft_strchr(EX_SPECIAL_VAR_CHAR, c));
}
