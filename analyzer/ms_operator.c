#include "ms_analyzer.h"

// 演算子文字列(長さ降順)
const char			*g_op_label[] = {
	"<<-",
	"<<",
	">>",
	"<&",
	">&",
	"<>",
	"&&",
	"||",
	"<",
	">",
	"|",
	"&",
	";",
	"(",
	")",
	NULL};

// 演算子 t_token_id
const t_token_id	g_op_token_id[] = {
	TI_LTLTHYPHEN,
	TI_LTLT,
	TI_GTGT,
	TI_LTAND,
	TI_GTAND,
	TI_LTGT,
	TI_ANDAND,
	TI_PIPEPIPE,
	TI_LT,
	TI_GT,
	TI_PIPE,
	TI_AND,
	TI_SEMICOLON,
	TI_PAREN_L,
	TI_PAREN_R,
	0};

// 演算子 長さ
const size_t		g_op_len[] = {
	3,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	0};

// 全 token_id
const t_token_id	g_all_token_id[] = {
	TI_WORD,
	TI_IO_NUMBER,
	TI_NAME,
	TI_ASSIGNMENT_WORD,
	TI_SUBSHELL,
	// [リダイレクション演算子]
	TI_LT, // <
	TI_GT, // >
	TI_LTLT, // <<
	TI_GTGT, // >>
	TI_LTGT, // <>
	TI_LTAND, // <&
	TI_GTAND, // >&
	TI_LTLTHYPHEN, // <<-
	// [clause終端演算子]
	TI_PIPE, // |
	// [パイプライン終端演算子]
	TI_ANDAND, // &&
	TI_PIPEPIPE, // ||
	TI_AND, // &
	TI_SEMICOLON, // ;
	// [サブシェル開始・終了演算子]
	TI_PAREN_L, // (
	TI_PAREN_R, // )
	TI_NONE,
	0};

// 全 token_idラベル
const char			*g_all_token_label[] = {
	"WORD",
	"IO_NUMBER",
	"NAME",
	"ASSIGNMENT_WORD",
	"SUBSHELL",
	// [リダイレクション演算子]
	"<",
	">",
	"<<",
	">>",
	"<>",
	"<&",
	">&",
	"<<-",
	// [clause終端演算子]
	"|",
	// [パイプライン終端演算子]
	"&&",
	"||",
	"&",
	";",
	// [サブシェル開始・終了演算子]
	"(",
	")",
	"NONE",
	NULL};

// アクティブなLexerトークンがなんらかの演算子なら1を返す
// returns 1 if active t_wdlist is an operator.
int	lx_is_an_operator(t_lex_cursor *cursor)
{
	const char	*strhead;
	int			i;
	size_t		d;

	if (!cursor || !cursor->tail)
		return (0);
	strhead = cursor->line + cursor->tail->i;
	i = -1;
	while (g_op_label[++i])
	{
		d = cursor->i - cursor->tail->i;
		if (d == g_op_len[i]
			&& !ft_strncmp(strhead, g_op_label[i], g_op_len[i]))
			return (1);
	}
	return (0);
}

// `line[i]`から最も長い演算子が取れるまで`i`を進める
size_t	lx_cut_operator(t_lex_cursor *cursor)
{
	size_t	k;
	size_t	n;

	k = 0;
	while (g_op_label[k])
	{
		n = ft_starts_with(&(cursor->line[cursor->i]), g_op_label[k]);
		if (n)
			return (n);
		k += 1;
	}
	return (0);
}

// Lexer演算子トークンに対応するParserトークンIDを得る
// returns a t_token_id for given t_wdlist(only for operator)
t_token_id	pa_operator_token_id(t_wdlist *word)
{
	int	i;

	i = -1;
	while (g_op_label[++i])
	{
		if (!ft_strncmp(word->word, g_op_label[i], g_op_len[i]))
			return (g_op_token_id[i]);
	}
	return (TI_NONE);
}

// Parser演算子トークンIDのラベル文字列を得る
// returns a label string for given t_token_id(only for operator)
const char	*pa_operator_label(t_token_id ti)
{
	int	i;

	i = -1;
	while (g_op_token_id[++i])
	{
		if (g_op_token_id[i] == ti)
			return (g_op_label[i]);
	}
	return (NULL);
}

// ParserトークンIDのラベル文字列を得る
// returns a label string for given t_token_id
const char	*pa_token_label(t_token_id ti)
{
	int	i;

	i = -1;
	while (g_all_token_id[++i])
	{
		if (g_all_token_id[i] == ti)
			return (g_all_token_label[i]);
	}
	return (NULL);
}
