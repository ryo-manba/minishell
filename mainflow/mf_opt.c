/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mf_opt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 18:45:18 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/15 09:23:26 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	trap_c(t_master *master, t_msopt opt)
{
	if (opt == MSO_C)
		return (mf_mark_failed(master, 2, "-c: option requires an argument"));
	return (1);
}

static int	trap_bare(t_master *master, t_msopt opt, char *argv)
{
	if (opt == MSO_C && !master->spcursor.c_line)
		master->spcursor.c_line = argv;
	else if (opt != MSO_C && !master->filepath)
		master->filepath = argv;
	return (1);
}

t_msopt	mf_opt_for_arg(char *argv)
{
	if (ft_strcmp(argv, "-c") == 0)
		return (MSO_C);
	return (MSO_BARE);
}

int	mf_setup_opt(t_master *master)
{
	if (master->opt_c && master->spcursor.c_line)
	{
		master->spcursor.lines = ft_split(master->spcursor.c_line, '\n');
		if (!master->spcursor.lines)
			return (mf_mark_failed(master, 1, "[exec-c] split"));
	}
	else if (master->filepath)
	{
		errno = 0;
		if (ms_open_at(STDIN_FILENO, master->filepath, O_RDONLY, -1))
		{
			mf_print_errno(master, 127, errno);
			return (master->failed);
		}
		master->prog_name = master->filepath;
	}
	return (MS_MS_SUCC);
}

int	mf_parse_opt(t_master *master, int argc, char **argv)
{
	t_msopt	opt;
	t_msopt	nopt;
	int		i;

	opt = MSO_DUMMY;
	i = 0;
	while (++i < argc)
	{
		nopt = mf_opt_for_arg(argv[i]);
		if (nopt == MSO_C && trap_c(master, opt))
		{
			opt = MSO_C;
			continue ;
		}
		else if (nopt == MSO_BARE && trap_bare(master, opt, argv[i]))
		{
			opt = MSO_BARE;
			break ;
		}
	}
	if (opt == MSO_C)
		return (mf_mark_failed(master, 2, "-c: option requires an argument"));
	master->opt_c = !!master->spcursor.c_line;
	return (mf_setup_opt(master));
}
