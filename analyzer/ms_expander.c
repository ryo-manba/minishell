#include "ms_analyzer.h"

static void	init_ex_cursor(t_ex_cursor *cursor, t_stree *src)
{
	ft_bzero(cursor, sizeof(t_ex_cursor));
	cursor->src.head = src;
	cursor->src.tail = src;
}

void	concat_stree_cursor(t_ex_cursor *cursor, t_stree *res)
{
	if (!cursor->res.head)
		cursor->res.head = res;
	if (!cursor->res.tail)
		cursor->res.tail = res;
	else
		cursor->res.tail->right = res;
	while (cursor->res.tail->right)
		cursor->res.tail = cursor->res.tail->right;
}

t_redir	*ms_expand_redir(t_ex_state *state, t_redir *redir)
{
	t_redir	*tail;
	t_stree	*original;
	t_stree	*expanded;

	tail = redir;
	while (tail)
	{
		if (!tail->operand_right)
			return ((t_redir *)ex_fatal(state, "redir has no right"));
		original = tail->operand_right;
		expanded = ms_expand_stree(state, original);
		if (!expanded || expanded->right)
			return ((t_redir *)ex_error(state, original, "ambiguous redirect"));
		tail->operand_right = expanded;
		tail = tail->next;
	}
	return (redir);
}

t_stree	*ms_expand_stree(t_ex_state *state, t_stree *src)
{
	t_ex_cursor	cursor;
	t_ex_token	*res;
	t_stree		*st;

	init_ex_cursor(&cursor, src);
	while (cursor.src.tail)
	{
		printf("[LL]\n");
		res = ex_shell_param(state, cursor.src.tail);
		ex_stringify_extoken(res);
		if (!state->no_split || cursor.src.tail->token_id != TI_ASSIGNMENT_WORD)
		{
			printf("[SP]\n");
			res = ex_split(state, res);
		}
		ex_stringify_extoken(res);
		if (cursor.src.tail->token_id != TI_ASSIGNMENT_WORD)
		{
			printf("[FX]\n");
			res = ex_fx(state, res);
		}
		ex_stringify_extoken(res);
		printf("[JO]\n");
		st = ex_join(state, res);
		concat_stree_cursor(&cursor, st);
		cursor.src.tail = cursor.src.tail->right;
	}
	pa_destroy_stree(cursor.src.head);
	return (cursor.res.head);
}
