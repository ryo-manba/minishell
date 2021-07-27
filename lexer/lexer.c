#include "lexer.h"

void	debug(t_word_list *lst)
{
	while (lst)
	{
		printf("token -> [%s]\nstate -> [%d]\n", lst->word, lst->detail_type);
		lst = lst->next;
	}
}

void	free_stack(t_stack *sta)
{
	t_stack	*tmp;

	while (sta)
	{
		tmp = sta->next;
		free(sta);
		sta = tmp;
	}
}

void	free_head_lst(t_word_list **lst)
{
	t_word_list *tmp;

	tmp = (*lst)->next;
	free((*lst)->word);
	free(*lst);
	*lst = tmp;
}

bool	update_lst_accordingly_type(t_stack *sta, t_word_list *word_lst, char *s, int type)
{
	if (type == TYPE_SQUOTE || type == TYPE_DQUOTE)
	{
		while (1)
		{
			sta->i++;
			if (s[sta->i] == '\0')
			{
				printf("open quote is not supported...\n");
				return (false);
			}
			sta_push_back(sta, s[sta->i]);
			if (type == check_word(s[sta->i]))
			{
				lst_push_back(word_lst, create_word(sta, sta->start, sta->i), type);
				sta->start = sta->i + 1;
				break ;
			}
		}
	}
	else if (TYPE_DOLLAR <= type && type <= REDIRECT_OUTPUT) //もう一個先をみる
	{
//		printf("i -> %d start -> %d\n", sta->i, sta->start);
		lst_push_back(word_lst, create_word(sta, sta->start, sta->i), type);
	}
	else if (type == TYPE_SPACE)
	{
		lst_push_back(word_lst, create_word(sta, sta->start, sta->i - 1), type);
//		while (s[sta->i] && check_word(s[sta->i]) == TYPE_SPACE) // spaceの間飛ばす
//		{
//			sta->i++;
//			printf("i -> %d start -> %d\n", sta->i, sta->start);
//			sleep(1);
//		}
 		sta->start = sta->i + 1;
//		printf("finish             i -> %d start -> %d\n", sta->i, sta->start);
	}
	return (true);
}

t_word_list	*lexer(char *s)
{
	t_stack		*sta;
	t_word_list	*word_lst;
	int			type;

	printf("args -> %s\n", s);

	sta = stack_new(s[0]);
	word_lst = lst_new(NULL, 0);
	while (1)
	{
		type = check_word(s[sta->i]);
		if (TYPE_SPACE <= type && type <= REDIRECT_OUTPUT)
		{
			if (!update_lst_accordingly_type(sta, word_lst, s, type))
				return (NULL);
		}
		if (s[sta->i] == '\0')
		{
			lst_push_back(word_lst, create_word(sta, sta->start, sta->i - 1), type);
			break ;
		}
		sta->i++;
		sta_push_back(sta, s[sta->i]);
	}
	free_head_lst(&word_lst);
	debug(word_lst);
	free_stack(sta);
	return (word_lst);
}

//isspaceみたいなの必要
int	main()
{
	lexer("echo hello world");
//	lexer("echo hello world || cat res.text || grep hello");
//	lexer("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
//	lexer("echo \"hello world\"");
//	lexer("echo \'hello world\'");
//	lexer("echo \'h\"\"\"\"ello world\'");
//	lexer("echo \"\"\"     \"\'hello world\'");
//	lexer("echo \"\"\"     \"\'hello world\'");
//	lexer("echo \"\'hello world\'\"");
//	lexer("echo \'\"hello world hello\"\'");
	lexer("& | $  * < >");
//	lexer("echo \"hello\"\"world\"");
//	lexer("echo \'hello world");
//	lexer("echo \"hello world");

	return (0);
}