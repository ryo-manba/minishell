#include "ms_analyzer.h"

static t_wdlist	*new_lexer_token(int i, char starting_char)
{
	t_wdlist	*new;

	new = (t_wdlist *)ft_calloc(1, sizeof(t_wdlist));
	if (new)
	{
		new->i = i;
		new->starting_chartype = starting_char;
		new->lex_type = LT_TOKEN;
	}
	return (new);
}

// 追加できる状態なら追加。そうでなければなにもしない。
// returns 1 if it failed
int	lx_add_token(t_lex_cursor *cursor, char ct)
{
	t_wdlist	*new;

	if (cursor->tail && !(cursor->tail->concluded))
		return (MS_AZ_SUCC);
	new = new_lexer_token(cursor->i, ct);
	if (!new)
	{
		cursor->failed = 1;
		return (MS_AZ_FAIL);
	}
	if (cursor->tail)
		cursor->tail->next = new;
	cursor->tail = new;
	if (!cursor->head)
		cursor->head = cursor->tail;
	cursor->tail->word = cursor->line + cursor->tail->i;
	printf("added %p at %d: starts with '%c'\n", cursor->head, cursor->i, ct);
	return (MS_AZ_SUCC);
}

// Lexerトークンを閉じる
// always succeeds
void	lx_conclude_token(t_lex_cursor *cursor)
{
	t_wdlist	*tail;
	char		delimiter;

	tail = cursor->tail;
	if (!tail || tail->concluded)
		return ;
	delimiter = cursor->line[cursor->i];
	tail->delimiter = delimiter;
	tail->len = cursor->i - tail->i;
	tail->concluded = 1;
	if (tail->starting_chartype == LC_NEWLINE)
		tail->lex_type = LT_NEWLINE;
	else if (lx_is_an_operator(cursor))
		tail->lex_type = LT_OPERATOR;
	else if (ft_strchr("<>", tail->delimiter)
		&& lx_is_digital_str(tail->word, tail->len))
		cursor->tail->lex_type = LT_IO_NUMBER;
	printf("concluded %p at %d: type is %d\n", tail, cursor->i, tail->lex_type);
}
