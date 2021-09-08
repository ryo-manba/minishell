/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_analyzer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 01:40:19 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/07 00:59:46 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_ANALYZER_H
# define TEST_ANALYZER_H
# include "ms_analyzer.h"

void	print_pipeline(t_pipeline *pipeline, int depth);
void	print_clause(t_clause *clause, int depth);
void	print_redir(t_redir *redir, int depth);
void	print_stree(t_stree *stree, int depth);

#endif
