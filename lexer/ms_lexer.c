#include "ms_lexer.h"

static void debug(t_word_list *lst, char *s)
{
	const char	*token[3] = {"RESERVED", "WORD", "OPERATOR"};
	const char	*type[13] =
				{"NORMAL", "SPACE", "SINGLE_QUOTE",
				"DOUBLE_QUOTE","AND", "PIPE",
				"WILD","DOUBLE_AND", "DOUBLE_PIPE",
				"HEREDOC", "REDIRECT_INPUT",
				"REDIRECT_OUTPUT", "REDIRECT_APPEND"};
	while (lst)
	{
		printf(
		"-----------------------------------\n"
		"word -> [%s]\n"
		"type -> [%s][%s]\n"
		"", lst->word, token[lst->token_type], type[lst->detail_type]);
		lst = lst->next;
	}
	printf("-----------------------------------\n");
}

// クォートが閉じられているか調べながら区切り文字まで進める。
bool	ms_check_open_quote_and_delimiter(t_word_list *lst, char *s)
{
	int	sq;
	int	dq;

	sq = 0;
	dq = 0;
	while (ms_check_delimiter(s[lst->i + 1]) == false)
	{
		if (ms_check_word_type(s[lst->i]) == TYPE_SINGLE_QUOTE)
			sq++;
		if (ms_check_word_type(s[lst->i]) == TYPE_DOUBLE_QUOTE)
			dq++;
		lst->i++;
	}
	if (sq % 2 == 0 && dq % 2 == 0)
		return (true);
	return (false);
}

// quote内の処理
void	ms_in_quoting(t_word_list *lst, char *s, int type)
{
	while (s[lst->i++])
	{
		if (ms_check_word_type(s[lst->i]) == type) // ここから次のspaceが来るまで読む
		{
			if (ms_check_open_quote_and_delimiter(lst, s) == true)
			{
				ms_lst_push_back(lst, ms_create_word(s, lst->start, lst->i), WORD, type);
				return ;
			}
		}
	}
	ms_error_exit("open quote is not supported...\n");
}

// タイプに合わせてリストを更新する。
void ms_update_lst_accordingly_type(t_word_list *lst, char *s, int type)
{
	if (type == TYPE_SINGLE_QUOTE || type == TYPE_DOUBLE_QUOTE)
	{
		ms_in_quoting(lst, s, type);
	}
	else if (TYPE_AND <= type && type <= REDIRECT_OUTPUT)
	{
		ms_check_operator_and_update_lst(lst, s, type);
	}
	else if (type == TYPE_SPACE)
	{
		if (lst->start < lst->i)
		{
			ms_lst_push_back(lst, ms_create_word(s,
				lst->start, lst->i - 1), WORD, ms_check_word_type(s[lst->i - 1])); // typeはspaceの前の文字
		}
		lst->start = lst->i + 1; // spaceが続いて場合に次のspaceをリストに含まないようにする
	}
}

t_word_list *ms_lexer(char *s)
{
	t_word_list *lst;
	int 		type;

	lst = ms_lst_new(NULL, 0, 0); // 先頭はダミーノード(push_backで繋げていきたいので)
	while (1)
	{
		type = ms_check_word_type(s[lst->i]);
		if (TYPE_SPACE <= type && type <= REDIRECT_OUTPUT)
		{
			ms_update_lst_accordingly_type(lst, s, type);
		}
		if (s[lst->i] == '\0')
		{
			if (lst->start < lst->i)
				ms_lst_push_back(lst, ms_create_word(s, lst->start, lst->i - 1), WORD, type);
			break;
		}
		lst->i++;
	}
	ms_free_head_lst(&lst);
	debug(lst, s);
	return (lst);
}
