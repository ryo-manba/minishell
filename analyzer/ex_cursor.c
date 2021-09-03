#include "ms_analyzer.h"

void	ex_add_token_csr(t_ex_part_cursor *cursor, t_ex_token *ext)
{
	if (cursor->tail)
		cursor->tail->right = ext;
	cursor->tail = ext;
	if (!cursor->head)
		cursor->head = cursor->tail;
}

t_ex_token	*ex_pop_src_token_csr(t_ex_part_cursor *cursor)
{
	if (cursor->tail)
		cursor->tail = cursor->tail->right;
	return (cursor->tail);
}

void	ex_init_sp_cursor(t_ex_cursor *cursor, t_token_id tid, const char *str)
{
	ft_bzero(cursor, sizeof(t_ex_cursor));
	cursor->str = str;
	cursor->n = ft_strlen(str);
	cursor->running = XI_BARE;
	cursor->pa_token_id = tid;
}

void	ex_init_sw_cursor(t_ex_cursor *cursor, t_ex_token *ext)
{
	ft_bzero(cursor, sizeof(t_ex_cursor));
	cursor->p.src_head = ext;
	cursor->p.src_tail = ext;
}

