#include "lexer.h"

static void debug(t_word_list *lst, char *s)
{
	printf("args -> %s\n", s);
	while (lst)
	{
		printf(
		"-------------------------\n"
		"word  -> [%s]\n"
		"token  -> [%d]\n"
		"detail -> [%d]\n"
		"", lst->word, lst->token_type, lst->detail_type);
		lst = lst->next;
	}
	printf("-------------------------\n");
}

void free_head_lst(t_word_list **lst)
{
	t_word_list *tmp;

	tmp = (*lst)->next;
	free((*lst)->word);
	free(*lst);
	*lst = tmp;
}

bool	in_quoting(t_word_list *lst, char *s, int type)
{
	while (1)
	{
		lst->i++;
		if (s[lst->i] == '\0')
		{
			printf("open quote is not supported...\n");
			return (false);
		}
		if (type == check_word(s[lst->i]))
		{
			lst_push_back(lst, create_word(s, lst->start, lst->i), WORD, type);
			lst->start = lst->i + 1;
			break;
		}
	}
	return (true);
}


void	check_operator_and_create_lst(t_word_list *lst, char *s, int type)
{
	int	j;

	j = 0;
	if (lst->start < lst->i) // 後ろがスペースとかだったら何も入れない
		lst_push_back(lst, create_word(s, lst->start, lst->i - 1), WORD, check_word(s[lst->i - 1]));
	if (type == REDIRECT_INPUT && check_word(s[lst->i+1]) == REDIRECT_OUTPUT) // <>
	{
		lst_push_back(lst, create_word(s, lst->i, lst->i+1), OPERATOR, TYPE_NORMAL);
		lst->i++;
	}
	else
	{
		while (type == check_word(s[lst->i + 1]))
		{
			lst->i++;
			j++;
		}
		if (j != 0 && j % 2 == 1) // 偶数だったら2個ずつ入れてく
		{
			type = check_type(type);
			while (j > 0)
			{
				lst_push_back(lst, create_word(s, lst->i - j, lst->i - j + 1), OPERATOR, type);
				j -= 2;
			}
		}
		else // 奇数の場合とoperatorがつながってない場合
			lst_push_back(lst, create_word(s, lst->i - j, lst->i), OPERATOR, type);
		}
	lst->start = lst->i + 1;
}

bool update_lst_accordingly_type(t_word_list *lst, char *s, int type)
{
	if (type == TYPE_SINGLE_QUOTE || type == TYPE_DOUBLE_QUOTE)
	{
		if (!in_quoting(lst, s, type))
			return (false);
	}
	else if (TYPE_AND <= type && type <= REDIRECT_OUTPUT)
		check_operator_and_create_lst(lst, s, type);
	else if (type == TYPE_SPACE)
	{
		if (lst->start < lst->i)
			lst_push_back(lst, create_word(s, lst->start, lst->i - 1), WORD, check_word(s[lst->i - 1]));
		lst->start = lst->i + 1; // startはspaceの次から
	}
	return (true);
}

t_word_list *lexer(char *s)
{
	t_word_list *lst;
	int 		type;

	lst = lst_new(NULL, 0, 0); // 先頭 dummy node
	while (1)
	{
		type = check_word(s[lst->i]);
		if (TYPE_SPACE <= type && type <= REDIRECT_OUTPUT)
		{
			if (!update_lst_accordingly_type(lst, s, type))
				return (NULL);
		}
		if (s[lst->i] == '\0')
		{
			if (lst->start < lst->i)
				lst_push_back(lst, create_word(s, lst->start, lst->i - 1), WORD, type);
			break;
		}
		lst->i++;
	}
	free_head_lst(&lst);
	debug(lst, s);
	return (lst);
}
