/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 13:38:32 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/17 16:59:42 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../common/ms_common.h"
# include "../analyzer/ms_analyzer.h"
# include "../analyzer/test_analyzer.h"
# include "../utils/ms_utils.h"
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

# define MS_MS_FAIL 1
# define MS_MS_SUCC 0
# ifndef MS_PROMPT
#  define MS_PROMPT "% "
# endif

extern volatile sig_atomic_t	g_ex_states;
void		ms_sigint_handler(int sig);

typedef enum e_msopt
{
	MSO_DUMMY,
	MSO_C,
	MSO_I,
	MSO_BARE,
}	t_msopt;

int			mf_parse_opt(t_master *master, int argc, char **argv);
int			mf_print_errno(t_master *master, int mark, int given_errno);
int			mf_mark_failed(t_master *master, int mark, char *message);
void		mf_loop(t_master *master);
t_pipeline	*mf_parse(t_master *master, char *line, t_parse_state *ps);

#endif
