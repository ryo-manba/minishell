#include "lexer.h"

int	lexer(char *s)
{
	t_stack		*sta;
	t_word_list	*word_lst;
	int 		i;
	int			start;
	int			type;
	bool 		lst_flag = true;

	i = 0;
	start = 0;
	printf("args -> %s\n", s);

	sta = stack_new(s[i]);
	while (s[sta->i])
	{
		type = check_word(s[sta->i]);
		if (type == TYPE_DQUOTE) // 次のDQUOTEが来るまで進める
		{
			sta->i++;
			while (1)
			{
				if (s[sta->i] == '\0')
				{
					printf("open quote is not supported...\n");
					return (1);
				}
				type = check_word(s[sta->i]);
				sta_push_back(sta, s[sta->i]);
				if (type == TYPE_DQUOTE)
				{
					lst_push_back(word_lst, create_word(sta, sta->start, sta->i), TYPE_DQUOTE);
					sta->start = sta->i + 1;
					break ;
				}
				sta->i++;
			}
		}
		if (type == TYPE_SQUOTE) // 次のSQUOTEが来るまですすむ
		{
			sta->i++;
			while (1)
			{
				if (s[sta->i] == '\0')
				{
					printf("open quote is not supported...\n");
					return (1);
				}
				type = check_word(s[sta->i]);
				sta_push_back(sta, s[sta->i]);
				if (type == TYPE_SQUOTE)
				{
					lst_push_back(word_lst, create_word(sta, sta->start, sta->i), TYPE_SQUOTE);
					sta->start = sta->i + 1;
					break ;
				}
				sta->i++;
			}
		}
		if (type == TYPE_SPACE)
		{
			if (lst_flag)
			{
				word_lst = wordlst_new(create_word(sta, sta->start, sta->i-1), TYPE_NORMAL);
				lst_flag = false;
			}
			else
				lst_push_back(word_lst, create_word(sta, sta->start, sta->i-1), TYPE_NORMAL);
			sta->start = sta->i + 1;
		}
		sta->i++;
		sta_push_back(sta, s[sta->i]);
	}

	if (sta->start != sta->i)
	{
		if (lst_flag)
			word_lst = wordlst_new(create_word(sta, sta->start, sta->i - 1), TYPE_NORMAL);
		else
			lst_push_back(word_lst, create_word(sta, sta->start, sta->i - 1), TYPE_NORMAL);
	}
	while (word_lst)
	{
		printf("token -> [%s]\nstate -> [%d]\n", word_lst->word, word_lst->flags);
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
//	lexer("echo hello world");
//	lexer("echo hello world || cat res.text || grep hello");
//	lexer("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
//	lexer("echo \"hello world\"");
//	lexer("echo \'hello world\'");
//	lexer("echo \"\'hello world\'\"");
//	lexer("echo \'\"hello world hello\"\'");
	lexer("& | $  * < > ");
	lexer("echo \"hello\"\"world\"");
	lexer("echo \'hello world");
	lexer("echo \"hello world");


	return (0);
}
