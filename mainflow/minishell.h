/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 13:38:32 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/12 11:55:17 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../analyzer/ms_analyzer.h"
# include "../analyzer/test_analyzer.h"
# include "../utils/ms_utils.h"
# include <readline/readline.h>
# include <readline/history.h>

# include "../common/ms_common.h"

# define MS_MS_FAIL 1
# define MS_MS_SUCC 0

extern volatile sig_atomic_t	g_ex_states;
void	ms_sigint_handler(int sig);

typedef enum e_msopt
{
	MSO_DUMMY,
	MSO_C,
	MSO_BARE,
}	t_msopt;

#endif
