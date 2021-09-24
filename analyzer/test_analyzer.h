/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_analyzer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 01:40:19 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/25 01:15:49 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_ANALYZER_H
# define TEST_ANALYZER_H
# include "ms_parser.h"

void	print_pipeline(t_master *master, t_pipeline *pipeline, int depth);
void	print_clause(t_master *master, t_clause *clause, int depth);
void	print_redir(t_master *master, t_redir *redir, int depth);
void	print_stree(t_master *master, t_stree *stree, int depth);
void	print_words(t_wdlist *words);

#endif
