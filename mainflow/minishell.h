/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 13:38:32 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/11 16:14:21 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../analyzer/ms_analyzer.h"
# include "../analyzer/test_analyzer.h"
# include "../utils/ms_utils.h"
# include <readline/readline.h>
# include <readline/history.h>

extern volatile sig_atomic_t	g_ex_states;

#endif
