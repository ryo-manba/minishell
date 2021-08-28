#include "ms_analyzer.h"

const char	g_type_chars[] = {
	LC_WORD,
	LC_NEWLINE,
	LC_SINGLE_QUOTE,
	LC_DOUBLE_QUOTE,
	LC_SPACE,
	LC_SPACE,
	LC_REDIRECT_INPUT,
	LC_REDIRECT_OUTPUT,
	LC_AND,
	LC_PIPE,
	LC_SEMICOLON,
	LC_PAREN_L,
	LC_PAREN_R,
	'\0'};

// lexerのうちクオート関連処理
// 引っかからなかった場合は0を返す。
static int	treat_quote(t_lex_cursor *cursor, char c, char ct)
{
	if (c == cursor->under_quoted)
		cursor->under_quoted = '\0';
	else if (!cursor->under_quoted
		&& (ct == LC_SINGLE_QUOTE || ct == LC_DOUBLE_QUOTE))
	{
		cursor->under_quoted = ct;
		if (lx_add_token(cursor, ct))
			return (1);
	}
	else if (cursor->under_quoted || ct == LC_WORD)
	{
		if (cursor->tail
			&& !ft_strchr(CHARS_WORD_INCLUDED, cursor->tail->starting_chartype))
			lx_conclude_token(cursor);
		if (lx_add_token(cursor, ct))
			return (1);
		cursor->i += 1;
		return (1);
	}
	else
		return (0);
	cursor->i += 1;
	return (1);
}

// lexerのうちNEWLINE処理
// 引っかからなかった場合は0を返す。
static int	treat_nl(t_lex_cursor *cursor, char c, char ct)
{
	(void)c;
	if (ct != LC_NEWLINE)
		return (0);
	lx_conclude_token(cursor);
	if (lx_add_token(cursor, ct))
		return (1);
	cursor->i += 1;
	return (1);
}

// lexerのうち空白処理
// 引っかからなかった場合は0を返す。
static int	treat_space(t_lex_cursor *cursor, char c, char ct)
{
	(void)c;
	if (ct != LC_SPACE)
		return (0);
	lx_conclude_token(cursor);
	cursor->i += 1;
	return (1);
}

// lexerのうち演算子関連処理
// 引っかからなかった場合は0を返す。
static int	treat_operator(t_lex_cursor *cursor, char c, char ct)
{
	if (!ft_strchr("|&<>;()", c))
		return (0);
	if (!cursor->tail || cursor->tail->concluded)
		if (lx_add_token(cursor, ct))
			return (1);
	if (!ft_strchr("|&<>;()", cursor->line[cursor->tail->i]))
	{
		lx_conclude_token(cursor);
		if (lx_add_token(cursor, ct))
			return (1);
	}
	cursor->i += lx_cut_operator(cursor);
	lx_conclude_token(cursor);
	return (1);
}

t_wdlist	*ms_lexer(const char *line)
{
	t_lex_cursor	cursor;
	char			chartype;
	int				i;

	ft_bzero(&cursor, sizeof(t_lex_cursor));
	cursor.line = line;
	while (line[cursor.i])
	{
		if (cursor.failed)
			break ;
		i = ft_strchr_i("\n'\" \t<>&|;()", line[cursor.i]);
		chartype = g_type_chars[i + 1];
		if (treat_quote(&cursor, line[cursor.i], chartype))
			continue ;
		else if (treat_nl(&cursor, line[cursor.i], chartype))
			break ;
		else if (treat_space(&cursor, line[cursor.i], chartype))
			continue ;
		else if (treat_operator(&cursor, line[cursor.i], chartype))
			continue ;
		cursor.i += 1;
	}
	if (!cursor.failed)
		lx_conclude_token(&cursor);
	return (cursor.head);
}
