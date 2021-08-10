#include "ms_lexer.h"

int	ms_check_word_type(char c)
{
	if (c == '\'')
		return (TYPE_SINGLE_QUOTE);
	if (c == '\"')
		return (TYPE_DOUBLE_QUOTE);
	if (c == ' ')
		return (TYPE_SPACE);
	if (c == '|')
		return (TYPE_PIPE);
	if (c == '&')
		return (TYPE_AND);
	if (c == '*')
		return (TYPE_WILD);
	if (c == '<')
		return (REDIRECT_INPUT);
	if (c == '>')
		return (REDIRECT_OUTPUT);
	return (TYPE_NORMAL);
}

// 区切り文字だったらtrueを返す
bool	ms_check_delimiter(char c)
{
	int	type;

	type = ms_check_word_type(c);
	if (type == TYPE_SPACE)
		return (true);
	if (type == TYPE_AND)
		return (true);
	if (type == TYPE_PIPE)
		return (true);
	if (type == TYPE_WILD)
		return (true);
	if (type == REDIRECT_OUTPUT)
		return (true);
	if (type == REDIRECT_INPUT)
		return (true);
	if (c == '\0')
		return (true);
	return (false);
}

// 連続して意味を成す演算子か調べる
int	ms_check_consecutively_operator(int type)
{
	if (type == REDIRECT_INPUT)
		return (TYPE_HEREDOC);
	if (type == REDIRECT_OUTPUT)
		return (REDIRECT_APPEND);
	if (type == TYPE_PIPE)
		return (TYPE_DOUBLE_PIPE);
	if (type == TYPE_AND)
		return (TYPE_DOUBLE_AND);
	return (type);
}

// 演算子のチェックとリストの更新
void	ms_check_operator_and_update_lst(t_word_list *lst, char *s, int type)
{
	if (lst->start < lst->i) // 後ろがスペースとかだったら何も入れない
		ms_lst_push_back(lst, ms_create_word(s, lst->start, lst->i - 1), WORD, ms_check_word_type(s[lst->i - 1]));
	if (type == REDIRECT_INPUT && ms_check_word_type(s[lst->i + 1]) == REDIRECT_OUTPUT) // <>
	{
		lst->i++;
		ms_lst_push_back(lst, ms_create_word(s, lst->i - 1, lst->i), OPERATOR, TYPE_NORMAL);
	}
	else
	{
		ms_check_consecutively_and_update_lst(lst, s, type);
	}
}

// 演算子が続いてるか調べてリストを更新する。
// 2個ずつ分割する。
// >>>>> -> [>>][>>][>]
void	ms_check_consecutively_and_update_lst(t_word_list *lst, char *s, int type)
{
	int	j;

	j = 0;
	while (type == ms_check_word_type(s[lst->i + 1]))
	{
		lst->i++;
		j++;
	}
	if (j > 0) // 同じ演算子が続いた場合
	{
		type = ms_check_consecutively_operator(type);
		while (j > 0)
		{
			ms_lst_push_back(lst, ms_create_word(s, lst->i - j, lst->i - j + 1), OPERATOR, type);
			j -= 2;
			if (j == 0)
				ms_lst_push_back(lst, ms_create_word(s, lst->i, lst->i), OPERATOR, ms_check_word_type(s[lst->i]));
		}
	}
	else
		ms_lst_push_back(lst, ms_create_word(s, lst->i - j, lst->i), OPERATOR, type);
}
