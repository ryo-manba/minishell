#include "ms_analyzer.h"


t_stree	*ex_join(t_ex_state *state, t_ex_token *ext)
{
	t_ex_unit_cursor	csr;
	t_ex_token			*temp;
	t_stree				*st;
	char				*joined;

	ex_init_cursor_mid(&csr, ext);
	temp = NULL;
	while (1)
	{
		printf("csr.s.tail: %p\n", csr.s.tail);
		// exTokenがNULLでもDIVIDERでもない場合
		if (csr.s.tail && csr.s.tail->token_id != XI_DIVIDER)
		{
			if (!temp)
				temp = csr.s.tail;
			printf("temp: %p \"%s\" - \"%s\"\n", temp, temp->token, csr.s.tail->token);
		}
		else if (temp)
		{
			printf("str joining...\n");
			joined = ex_strcat_exlist(temp, 0);
			printf("joined: \"%s\"\n", joined);
			if (!joined)
			{
				state->failed = 1;
				break ;
			}
			st = ex_make_stree(joined, temp->pa_token_id);
			printf("st: %p\n", st);
			if (!st)
			{
				free(joined);
				state->failed = 1;
				break ;
			}
			if (csr.t.tail)
				csr.t.tail->right = st;
			csr.t.tail = st;
			if (!csr.t.head)
				csr.t.head = csr.t.tail;
			temp = NULL;
		}
		if (!csr.s.tail)
			break ;
		csr.s.tail = csr.s.tail->right;
	}
	ex_destroy_token(ext);
	if (state->failed)
	{
		pa_destroy_stree(csr.t.head);
		return (NULL);
	}
	return (csr.t.head);
}
