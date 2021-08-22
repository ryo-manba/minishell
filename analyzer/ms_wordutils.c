#include "ms_analyzer.h"

int	is_a_operator(t_wordcursor	*cursor)
{
	char	*strhead;

	if (!cursor || !cursor->tail)
		return (0);
	strhead = (char *)(cursor->line + cursor->tail->i);
	if (cursor->i - cursor->tail->i == 1)
	{
		if (!ft_strncmp(strhead, "<", 1) || !ft_strncmp(strhead, ">", 1) ||
			!ft_strncmp(strhead, "&", 1) || !ft_strncmp(strhead, ";", 1) ||
			!ft_strncmp(strhead, "(", 1) || !ft_strncmp(strhead, ")", 1) ||
			!ft_strncmp(strhead, "|", 1))
				return (1);
	}
	if (cursor->i - cursor->tail->i == 2)
	{
		if (!ft_strncmp(strhead, ">>", 2) || !ft_strncmp(strhead, "<<", 2) ||
			!ft_strncmp(strhead, "&&", 2) || !ft_strncmp(strhead, "||", 2) ||
			!ft_strncmp(strhead, "<&", 2) || !ft_strncmp(strhead, ">&", 2))
				return (1);
	}
	if (cursor->i - cursor->tail->i == 3)
		if (!ft_strncmp(strhead, "<<-", 3))
			return (1);
	return (0);
}

t_wdlist	*ms_new_lexer_token(int i, char starting_char)
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

void	ms_add_lexer_token(t_wordcursor *cursor, char starting_char)
{
	t_wdlist	*new;

	if (!cursor || (cursor->tail && !(cursor->tail->concluded)))
		return ;
	new = ms_new_lexer_token(cursor->i, starting_char);
	// error
	if (!new)
		return ;
	if (cursor->tail)
		cursor->tail->next = new;
	cursor->tail = new;
	if (!cursor->head)
		cursor->head = cursor->tail;
	printf("added %p at %d: starts with '%c'\n", cursor->head, cursor->i, starting_char);
}

void	ms_conclude_lexer_token(t_wordcursor *cursor)
{
	if (!cursor->tail || cursor->tail->concluded)
		return ;
	cursor->tail->concluded = 1;
	cursor->tail->word = ft_substr_range(cursor->line, cursor->tail->i, cursor->i);
	if (!cursor->tail->word)
		return ; // ERROR
	printf("(%d, %d) -> \"%s\"\n", cursor->tail->i, cursor->i, cursor->tail->word);
	// 区切り文字をセット
	if (cursor->line[cursor->i])
		cursor->tail->right_delimiter = cursor->line[cursor->i];
	// lex_typeの設定
	// 初期値はTOKEN
    if (cursor->tail->starting_chartype == LC_NEWLINE) {
		// 改行はトークン識別子NEWLINEになる。
		cursor->tail->lex_type = LT_NEWLINE;
    } else if (is_a_operator(cursor)) {
        // 演算子トークンは、その演算子トークンに対応するトークン識別子になる。
		cursor->tail->lex_type = LT_OPERATOR;
    } else if (ft_strchr("<>", cursor->tail->right_delimiter) &&
		ms_is_digital_str(cursor->tail->word)) {
        // 区切り文字が<>である数字のみのトークンは トークン識別子IO_NUMBER となる。
		cursor->tail->lex_type = LT_IO_NUMBER;
    }
	printf("concluded %p at %d: type is %d\n", cursor->tail, cursor->i, cursor->tail->lex_type);
}