#include "ms_analyzer.h"

// 演算子文字列(長さ降順)
const char	*g_operator_desc[] = {
	"<<-",
	"<<", ">>", "<&", ">&", "<>", "&&", "||",
	"<", ">", "|", "&", ";", "(", ")",
	NULL};

/**
 * 文字種別を判定する
 */
static char	ms_type_of_char(char c)
{
	if (c == '\n')
		return (LC_NEWLINE);
	if (c == '\'')
		return (LC_SINGLE_QUOTE);
	if (c == '"')
		return (LC_DOUBLE_QUOTE);
	if (c == ' ' || c == '\t')
		return (LC_SPACE);
	if (c == '<')
		return (LC_REDIRECT_INPUT);
	if (c == '>')
		return (LC_REDIRECT_OUTPUT);
	if (c == '&')
		return (LC_AND);
	if (c == '|')
		return (LC_PIPE);
	if (c == ';')
		return (LC_SEMICOLON);
	if (c == '(')
		return (LC_PAREN_L);
	if (c == ')')
		return (LC_PAREN_R);
	return (LC_WORD);
}

// lexerのうちクオート関連処理
// 引っかからなかった場合は0を返す。
int	ms_lex_treat_quote(t_wordcursor *cursor, char c, char ct)
{
	if (cursor->under_quoted && cursor->under_quoted == c)
		cursor->under_quoted = '\0'; // [クオート解除]
	else if (ct == LC_SINGLE_QUOTE)
	{
		// [シングルクオート]
		cursor->under_quoted = '\'';
		// トークンが開始していない場合は開始
		ms_add_lexer_token(cursor, ct);
	}
	else if (ct == LC_DOUBLE_QUOTE)
	{
		// [ダブルクオート]
		cursor->under_quoted = '"';
		// トークンが開始していない場合は開始
		ms_add_lexer_token(cursor, ct);
	}
	else if (cursor->under_quoted || ct == LC_WORD)
	{
		// [クオート下にあるクオートを解除しない文字]
		// or
		// [クオート下にない単語構成文字]
		// カレントトークンが単語でないならトークンを終了
		if (cursor->tail &&
			!ft_strchr(CHARS_WORD_INCLUDED, cursor->tail->starting_chartype))
			ms_conclude_lexer_token(cursor);
		ms_add_lexer_token(cursor, ct);
		cursor->i += 1;
		return (1);
		// [[ 以下全てクオート下でない ]]
	}
	else
		return (0);
	cursor->i += 1;
	printf("treated quote\n");
	return (1);
}

// lexerのうちNEWLINE処理
// 引っかからなかった場合は0を返す。
int	ms_lex_treat_nl(t_wordcursor *cursor, char c, char ct)
{
	(void)c;
	if (ct == LC_NEWLINE)
	{
		// [改行文字]
		// トークンを終了し、新しいトークンを開始した後解析を終了する。
		ms_conclude_lexer_token(cursor);
		ms_add_lexer_token(cursor, ct);
		cursor->i += 1;
		printf("treated NL\n");
		return (1);
	}
	return (0);
}

// lexerのうち空白処理
// 引っかからなかった場合は0を返す。
int	ms_lex_treat_space(t_wordcursor *cursor, char c, char ct)
{
	(void)c;
	if (ct == LC_SPACE)
	{
		ms_conclude_lexer_token(cursor);
		printf("treated Space\n");
		cursor->i += 1;
		return (1);
	}
	return (0);
}

/**
 * `line[i]`から"<"で始まる最も長い演算子が取れるまで`i`を進める
 */
size_t	cut_operator(t_wordcursor *cursor) {
	size_t	k;
	size_t	n;

	k = 0;
	while (g_operator_desc[k])
	{
		n = ft_starts_with(&(cursor->line[cursor->i]), g_operator_desc[k]);
		if (n)
			return (n);
		k += 1;
	}
	return (0);
}

// lexerのうち演算子関連処理
// 引っかからなかった場合は0を返す。
int	ms_lex_treat_operator(t_wordcursor *cursor, char c, char ct)
{
	if (!ft_strchr("|&<>;()", c))
		return (0);
	// [演算子構成文字、つまり <>&|;() のどれか]
	// - トークンが終了しているなら開始
	if (!cursor->tail || cursor->tail->concluded)
		ms_add_lexer_token(cursor, ct);
	// - そうでないなら、トークンの開始文字が演算子開始文字かどうかをチェック
	if (!ft_strchr("|&<>;()", cursor->line[cursor->tail->i]))
	{
		// - そうでないなら、ここでトークンを普通に切り、トークンを開始
		ms_conclude_lexer_token(cursor);
		ms_add_lexer_token(cursor, ct);
	}
	// トークンの開始文字が演算子開始文字なら、ここから最も長い演算子を切り出す。
	cursor->i += cut_operator(cursor);
	ms_conclude_lexer_token(cursor);
	printf("treated ope\n");
	return (1);
}

t_wdlist	*ms_lexer(const char *line)
{
	t_wordcursor	cursor;
	char			ct;

	cursor.head = NULL;
	cursor.tail = NULL;
	cursor.line = line;
	cursor.under_quoted = '\0';
	cursor.i = 0;
	while (line[cursor.i])
	{
		ct = ms_type_of_char(cursor.line[cursor.i]);
		// printf("[%d] cursor: (head %p tail %p quoted %d) '%c'\n", cursor.i, cursor.head, cursor.tail, cursor.under_quoted, ct);
		if (ms_lex_treat_quote(&cursor, cursor.line[cursor.i], ct))
			continue ;
		else if (ms_lex_treat_nl(&cursor, cursor.line[cursor.i], ct))
			break ;
		else if (ms_lex_treat_space(&cursor, cursor.line[cursor.i], ct))
			continue ;
		else if (ms_lex_treat_operator(&cursor, cursor.line[cursor.i], ct))
			continue ;
		cursor.i += 1;
	}
	ms_conclude_lexer_token(&cursor);
	return (cursor.head);
}
