/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_destroy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 19:01:35 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/05 19:01:47 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_analyzer.h"

void	ex_destroy_a_token(t_ex_token *ext)
{
	free((void *)ext->token);
	free(ext);
}

void	ex_destroy_token(t_ex_token *ext)
{
	if (!ext)
		return ;
	ex_destroy_token(ext->left);
	ex_destroy_token(ext->right);
	ex_destroy_a_token(ext);
}
