#include "ms_analyzer.h"

// factory funcs for parser.

t_stree	*ms_make_stree(t_wdlist *word)
{
	t_stree	*st;

	st = (t_stree *)ft_calloc(1, sizeof(t_stree));
	if (!st)
		return (NULL);
	st->token = ft_substr(word->word, 0, word->len);
	printf("st->token: %s [%d, %d)\n", st->token, word->i, word->i + word->len);
	if (!st->token)
	{
		free(st);
		return (NULL);
	}
	if (word->lex_type == LT_IO_NUMBER)
		st->token_id = TI_IO_NUMBER;
	else
		st->token_id = TI_WORD;
	return (st);
}

t_redir	*ms_make_redir(t_wdlist *op_word, t_stree *target, t_stree *ion)
{
	t_redir	*redir;

	redir = (t_redir *)ft_calloc(1, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->operand_left = ion;
	redir->operand_right = target;
	redir->redir_op = ms_operator_token_id(op_word);
	if (redir->redir_op == TI_NONE)
		return (NULL);
	return (redir);
}
