#include "ms_analyzer.h"

int	ms_syntax_error(t_parse_state *state, t_wdlist *word, char *message)
{
	state->err_word = word;
	state->err_message = message;
	return (MS_AZ_FAIL);
}
