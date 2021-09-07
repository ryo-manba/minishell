#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>


t_pipeline	*ms_lex_parse(char *line)
{
	t_wdlist	*lexer_token;
	t_parse_state	ps;

	lexer_token = ms_lexer(line);
	ms_parse(&ps, lexer_token, 0);
	lx_destroy_token(lexer_token);
	return (ps.pipeline);
}


int main(void)
{
	char *line;
	t_pipeline	*pipeline;

	setvbuf(stdout, (char *)NULL, _IONBF, 0);
	while (1)
	{
		line = readline("% ");
		pipeline = ms_lex_parse(line);
		print_pipeline(pipeline, 0);
		pa_destroy_pipeline(pipeline);
		add_history(line);
		free(line);
	}
}
