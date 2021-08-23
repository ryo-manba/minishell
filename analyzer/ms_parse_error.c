#include "ms_analyzer.h"

int	ms_return_with_error(t_parse_state *state, t_wdlist *word, char *message)
{
	state->error_word = word;
	state->error_message = message;
	return (MS_AZ_FAIL);
}
