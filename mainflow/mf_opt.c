/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mf_opt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 18:45:18 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/23 21:03:03 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_msopt	mf_opt_for_arg(char *argv)
{
	if (ft_strcmp(argv, "-c") == 0)
		return (MSO_C);
	if (ft_strcmp(argv, "-i") == 0)
		return (MSO_I);
	return (MSO_BARE);
}

int	mf_setup_opt(t_master *master)
{
	if (master->opt_c)
	{
		master->spcursor.c_line = master->filepath;
		master->filepath = NULL;
		master->spcursor.lines = ft_split_rough(master->spcursor.c_line, '\n');
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
	int		i;

	i = 0;
	while (++i < argc)
	{
		opt = mf_opt_for_arg(argv[i]);
		if (opt == MSO_BARE)
		{
			master->filepath = argv[i];
			break ;
		}
		else if (opt == MSO_C)
			master->opt_c = 1;
		else if (opt == MSO_I)
			master->opt_i = 1;
	}
	if (master->opt_c && !master->filepath)
		return (mf_mark_failed(master, 2, "-c: option requires an argument"));
	return (mf_setup_opt(master));
}
