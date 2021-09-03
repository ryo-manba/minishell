#include "ms_analyzer.h"

t_stree	*ex_join_words(t_ex_state *state, t_ex_token *ext)
{
	t_ex_unit_cursor	csr;

	ex_init_cursor_mid(&csr, ext);
	while (1)
	{
		if (!csr.s.tail || csr.s.tail->token_id == XI_DIVIDER)
		{
			if (csr.p.head || !csr.s.tail)
			{
				char		*subtoken;
				t_token_id	tid;
				t_stree		*st;

				subtoken = NULL;
				tid = TI_DUMMY;
				// if (csr.p.tail)
				// 	csr.p.tail->right = NULL;
				while (csr.p.head)
				{
					if (subtoken)
						subtoken = free_strjoin(subtoken, csr.p.head->token);
					else
						subtoken = ft_strdup(csr.p.head->token);
					if (tid == TI_DUMMY)
						tid = csr.p.head->pa_token_id;
					// TODO: Error
					if (csr.p.head == csr.p.tail)
						break ;
					csr.p.head = csr.p.head->right;
				}
				if (subtoken)
				{
					st = ex_make_stree(subtoken, tid);
					// TODO: Error
					if (csr.t.tail)
						csr.t.tail->right = st;
					csr.t.tail = st;
					if (!csr.t.head)
						csr.t.head = st;
					if (!csr.t.head)
						csr.t.head = csr.t.tail;
					csr.p.tail = NULL;
					csr.p.head = NULL; // TODO: destroy
				}
			}
		}
		else
		{
			if (csr.p.tail)
				csr.p.tail = csr.s.tail;
			csr.p.tail = csr.s.tail;
			if (!csr.p.head)
				csr.p.head = csr.p.tail;
		}
		if (!csr.s.tail)
			break ;
		csr.s.tail = csr.s.tail->right;
	}
}
