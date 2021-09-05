/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_analyzer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 01:40:19 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/06 01:40:20 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_ANALYZER_H
# define TEST_ANALYZER_H
# include "ms_analyzer.h"

void	print_pipeline(t_parse_state *state, t_pipeline *pipeline, int depth);
void	print_stree(t_parse_state *state, t_stree *stree, int depth);

#endif
