/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 13:38:28 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/12 18:38:17 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

void	ms_sigint_handler(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_ex_states = 1;
}

t_pipeline	*mf_lex_parse(char *line, t_parse_state *ps)
{
	t_lex_cursor	lx_cursor;
	t_wdlist		*lexer_token;

	lexer_token = ms_lexer(line, &lx_cursor);
	if (!lexer_token || lx_cursor.failed)
	{
		if (lx_cursor.failed)
			g_ex_states = lx_cursor.failed;
		lx_destroy_token(lexer_token);
		return (NULL);
	}
	print_words(lexer_token);
	ms_parse(ps, lexer_token, 0);
	lx_destroy_token(lexer_token);
	if (ps->failed)
	{
		pa_destroy_pipeline(ps->pipeline);
		g_ex_states = ps->failed;
		ps->pipeline = NULL;
	}
	return (ps->pipeline);
}

t_msopt	mf_opt_for_arg(char *argv)
{
	if (ft_strcmp(argv, "-c") == 0)
		return (MSO_C);
	return (MSO_BARE);
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
		if (nopt == MSO_C && opt == MSO_C)
			return (MS_MS_FAIL); // TODO: エラー表示
		else if (nopt == MSO_C)
			opt = MSO_C;
		else if (nopt == MSO_BARE)
		{
			if (opt == MSO_C && !master->c_line)
				master->c_line = argv[i];
			else if (opt != MSO_C && !master->filepath)
				master->filepath = argv[i];
			break ;
		}
	}
	master->opt_c = !!master->c_line;
	return (MS_MS_SUCC);
}

int	mf_init_master(t_master *master, int argc, char **argv)
{
	ft_bzero(master, sizeof(t_master));
	master->stdin_isatty = isatty(STDIN_FILENO);
	master->stderr_isatty = isatty(STDERR_FILENO);
	if (mf_parse_opt(master, argc, argv))
		return (MS_MS_FAIL);
	master->var = ms_create_env();
	master->args_given = argc - 1;
	if (!master->var)
		return (MS_MS_FAIL);
	master->interactive_shell = !master->opt_c
		&& master->args_given == 0
		&& master->stdin_isatty
		&& master->stderr_isatty;
	return (MS_MS_SUCC);
}

int	main(int argc, char **argv)
{
	char		*line;
	t_pipeline	*pipeline;
	t_parse_state	ps;
	t_ex_state	es;
	t_master	master;

	setvbuf(stdout, (char *)NULL, _IONBF, 0);
	if (signal(SIGINT, ms_sigint_handler) == SIG_ERR || \
		signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (1);
	if (mf_init_master(&master, argc, argv))
		return (1);
	master.var = ms_create_env();
	if (master.var == NULL)
	printf("c?: %d\n", master.opt_c);
	printf("c-line?: %s\n", master.c_line);
	printf("filepath?: %s\n", master.filepath);
	printf("interactive?: %d\n", master.interactive_shell);
	while (1)
	{
		line = readline("% ");
		if (!line)
			break ;
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			pipeline = mf_lex_parse(line, &ps);
			if (pipeline)
			{
				print_pipeline(pipeline, 0);
				ms_ex_init_state(&es, master.var, g_ex_states);
				int exec_status = ms_executer(pipeline, master.var, &es);
				if (es.failed)
					g_ex_states = es.failed;
				else if (exec_status)
					g_ex_states = exec_status;
				else
					g_ex_states = 0;
				pa_destroy_pipeline(pipeline);
			}
		}
		free(line);
	}
	printf("bye.\n");
}
