/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mf_repl.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 17:07:08 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/28 23:05:36 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	mf_loop_epilogue(t_master *master)
{
	ft_putstr_fd(master->prog_name, STDERR_FILENO);
	if (master->opt_c)
		ft_putstr_fd(": -c", STDERR_FILENO);
	ft_putstr_fd(": line ", STDERR_FILENO);
	ft_putsizet_fd(master->line_num, STDERR_FILENO);
	ft_putstr_fd(": `", STDERR_FILENO);
	ft_putstr_fd(master->current_line, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

static int	mf_read_line(t_master *master, char **line)
{
	int		status;

	*line = NULL;
	errno = 0;
	if (master->opt_c)
	{
		if (master->spcursor.lines[master->spcursor.i])
			*line = master->spcursor.lines[master->spcursor.i++];
		return (!*line);
	}
	if (master->filepath
		|| (!master->stdin_isatty && !master->interactive_shell))
	{
		status = get_next_line(STDIN_FILENO, line);
		if (status == -1)
			return (mf_print_errno(master, 126, errno));
		return (status == 0 && !*line);
	}
	*line = readline(MS_PROMPT);
	return (!*line);
}

static int	mf_execute(t_master *master, t_pipeline *pipeline)
{
	t_ex_state	es;

	ms_ex_init_state(&es, master);
	ms_executer_wrapper(pipeline, master, &es);
	return (g_ex_states);
}

void	mf_loop(t_master *master)
{
	char			*line;
	t_parse_state	ps;

	ft_bzero(&ps, sizeof(t_parse_state));
	while (!mf_read_line(master, &line))
	{
		mf_parse(master, line, &ps);
		if (ps.failed && !master->interactive_shell)
		{
			pa_destroy_pipeline(ps.pipeline);
			mf_loop_epilogue(master);
			break ;
		}
		safe_star_free((void **)&line);
		if (!ps.failed && ps.pipeline)
			mf_execute(master, ps.pipeline);
		pa_destroy_pipeline(ps.pipeline);
		ps.pipeline = NULL;
		master->line_num += 1;
		master->exited = 0;
	}
	safe_star_free((void **)&line);
}
