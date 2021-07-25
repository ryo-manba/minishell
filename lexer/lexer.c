#include "lexer.h"

int	lexer(char *s)
{
	t_stack		*sta;
	t_word_list	*word_lst;
	int 	i;
	int		start;
	int		type;
	int		sqort;
	int		dqort;
	bool 	lst_flag = true;

	i = 0;
	start = 0;
	sqort = 0;
	dqort = 0;

	sta = stack_new(s[i]);
	i++;
	while (s[i])
	{
		type = check_word(s[i]);
		sta_push_back(sta, s[i]);
		if (type == TYPE_SPACE)
		{
			if (lst_flag)
			{
				word_lst = wordlst_new(create_word(sta, start, i), TYPE_SPACE);
				lst_flag = false;
			}
			else
				lst_push_back(word_lst, create_word(sta, start, i), TYPE_SPACE);
			start = i + 1;
		}
		i++;
	}
	if (lst_flag)
		word_lst = wordlst_new(create_word(sta, start, i - 1), TYPE_SPACE);
	else
		lst_push_back(word_lst, create_word(sta, start, i - 1), TYPE_SPACE);
	while (word_lst)
	{
		printf("%s, %d\n", word_lst->word, word_lst->flags);
		word_lst = word_lst->next;
	}
	t_stack *tmp;
	while (sta)
	{
		tmp = sta->next;
		free(sta);
		sta = tmp;
	}
	return (0);
}

int	main()
{
	lexer("echo hello world");
	lexer("echo hello world || cat res.text || grep hello");
	lexer("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
//	lexer("echo ""hello world\"");
//	lexer("echo 'hello world'");
//	lexer("echo \"\'hello world\'");
//	lexer("echo \'\"hello world\"\'");
//	lexer("echo hello	                 world");
//	lexer("echo 'hello world'");


	return (0);
}
