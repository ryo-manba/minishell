#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

t_pipeline	*ms_lex_parse(char *line)
{
	t_wdlist	*lexer_token;
	t_parse_state	ps;

	lexer_token = ms_lexer(line);
	if (!lexer_token)
		return (NULL);
	ms_parse(&ps, lexer_token, 0);
	lx_destroy_token(lexer_token);
	return (ps.pipeline);
}

int main(void)
{
	char *line;
	t_pipeline	*pipeline;
	t_ex_state	es;

	setvbuf(stdout, (char *)NULL, _IONBF, 0);
	ms_ex_init_state(&es, NULL, 0);
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

				ms_executer(pipeline, NULL, &es);

				pa_destroy_pipeline(pipeline);
			}
		}
		free(line);
	}
}
