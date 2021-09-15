/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 13:38:28 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/15 09:36:41 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_sigint_handler(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_ex_states = 1;
}

void	mf_print_master(t_master *master)
{
	ft_putstr_fd("c?: ", STDERR_FILENO);
	ft_putnbr_fd(master->opt_c, STDERR_FILENO);
	ft_putstr_fd("\nc-line?: ", STDERR_FILENO);
	if (master->spcursor.c_line)
		ft_putstr_fd(master->spcursor.c_line, STDERR_FILENO);
	else
		ft_putstr_fd("(null)", STDERR_FILENO);
	ft_putstr_fd("\nfilepath?: ", STDERR_FILENO);
	if (master->filepath)
		ft_putstr_fd(master->filepath, STDERR_FILENO);
	else
		ft_putstr_fd("(null)", STDERR_FILENO);
	ft_putstr_fd("\ninteractive?: ", STDERR_FILENO);
	ft_putnbr_fd(master->interactive_shell, STDERR_FILENO);
	ft_putstr_fd("\nline?: ", STDERR_FILENO);
	ft_putsizet_fd(master->line_num, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

int	mf_init_master(t_master *master, int argc, char **argv)
{
	int	status;

	ft_bzero(master, sizeof(t_master));
	master->prog_name = argv[0];
	status = mf_parse_opt(master, argc, argv);
	if (status)
		return (status);
	master->var = ms_create_env();
	if (!master->var)
		return (MS_MS_FAIL);
	master->stdin_isatty = isatty(STDIN_FILENO);
	master->stderr_isatty = isatty(STDERR_FILENO);
	master->args_given = argc - 1;
	master->interactive_shell = !master->opt_c
		&& master->args_given == 0
		&& master->stdin_isatty
		&& master->stderr_isatty;
	if (!master->opt_c)
		master->line_num += 1;
	if (MS_DEBUG)
		mf_print_master(master);
	return (MS_MS_SUCC);
}

int	mf_destroy_master(t_master *master, int status)
{
	if (status)
		g_ex_states = status;
	else
		g_ex_states = master->failed;
	while (master->spcursor.lines
		&& master->spcursor.lines[master->spcursor.i])
		free(master->spcursor.lines[master->spcursor.i++]);
	free(master->spcursor.lines);
	free(master->var);
	if (MS_DEBUG)
	{
		ft_putstr_fd("bye with ", STDERR_FILENO);
		ft_putnbr_fd(g_ex_states, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	return (g_ex_states);
}

int	main(int argc, char **argv)
{
	t_master	master;
	int			status;

	setvbuf(stdout, (char *) NULL, _IONBF, 0);
	if (signal(SIGINT, ms_sigint_handler) == SIG_ERR || \
		signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (1);
	status = mf_init_master(&master, argc, argv);
	if (status)
		return (status);
	mf_loop(&master);
	return (mf_destroy_master(&master, 0));
}
