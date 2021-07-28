#include "lexer.h"

void debug(t_word_list *lst, char *s)
{
	printf("args -> %s\n", s);
	while (lst)
	{
		printf(
		"-------------------------\n"
		"token  -> [%s]\n"
		"state  -> [%d]\n"
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

bool update_lst_accordingly_type(t_word_list *lst, char *s, int type)
{
	int j;

	j = 0;
	if (type == TYPE_SQUOTE || type == TYPE_DQUOTE)
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
	}
	else if (TYPE_DOLLAR <= type && type <= REDIRECT_OUTPUT)
	{
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
			if (j != 1 && j % 2 == 1) // 偶数だったら2個ずつ入れてく
			{
				while (j > 0)
				{
					lst_push_back(lst, create_word(s, lst->i - j, lst->i - j + 1), OPERATOR, type);
					j -= 2;
				}
			}
			else // 奇数の場合とoperatorがつながってない場合など
				lst_push_back(lst, create_word(s, lst->i - j, lst->i), OPERATOR, type);
		}
		lst->start = lst->i + 1;
	}
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

int main()
{
//	lexer("echo hello world");
//	lexer("echo hello world || cat res.text || grep hello");
//	lexer("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
//	lexer("echo \"hello world\"");
//	lexer("echo 'hello world'");
//	lexer("echo \'h\"\"\"\"ello world\'");
//	lexer("echo \"\"\"     \"\'hello world\'");
//	lexer("echo \"\'hello world\'\"");
//	lexer("echo \'\"hello world hello\"\'");
//	lexer("echo hello|cat");
//	lexer("& | $                     * < >");
//	lexer("&& || << >> $$");
//	lexer("&&&&  $$$$$$$ ||||| <<<<  >>>> <>|$&&$$");
	lexer("echo<><><><><> <> <>");
//	lexer("$ $$$ $$$$$ $$$$$$$ $$$$$$$$$"); // 奇数
//	lexer("<< >>>> <<<<<< >>>>>>>> <<<<<<<<<<"); // 偶数
//	lexer("echo \"hello\"\"world\"");
//	lexer("echo \'hello world");
//	lexer("echo \"hello world");

	return (0);
}