/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 13:38:28 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/10 14:05:49 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

t_pipeline	*ms_lex_parse(char *line)
{
	t_wdlist		*lexer_token;
	t_parse_state	ps;

	lexer_token = ms_lexer(line);
	if (!lexer_token)
		return (NULL);
	ms_parse(&ps, lexer_token, 0);
	lx_destroy_token(lexer_token);
	return (ps.pipeline);
}

int	main(void)
{
	char		*line;
	t_pipeline	*pipeline;
	t_ex_state	es;
	t_shellvar	*var;

	setvbuf(stdout, (char *)NULL, _IONBF, 0);
	var = ms_create_env();
	if (var == NULL)
		return (1);
	ms_ex_init_state(&es, NULL, 0);
	es.var = ms_create_env();
	while (1)
	{
		line = readline("% ");
		if (!line)
			break ;
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			pipeline = ms_lex_parse(line);
			if (pipeline)
			{
				print_pipeline(pipeline, 0);
				ms_executer(pipeline, var, &es);
				pa_destroy_pipeline(pipeline);
			}
		}
		free(line);
	}
}
