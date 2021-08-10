#include "ms_lexer.h"

// エラーメッセージを出力してプロセスを終了する
void	ms_error_exit(char *error_message)
{
	printf("%s\n", error_message);
	exit(1);
}

// 先頭のダミーノードをfreeする
void ms_free_head_lst(t_word_list **lst)
{
	t_word_list *tmp;

	tmp = (*lst)->next;
	free((*lst)->word);
	free(*lst);
	*lst = tmp;
}

// push_backしてリストのstartを更新する
void	ms_lst_push_back(t_word_list *lst, char *s, int token_type, int detail_type)
{
	t_word_list	*new_lst;
	t_word_list	*tmp;

	new_lst = ms_lst_new(s, token_type, detail_type);
	if (!new_lst)
		ms_error_exit("Malloc error\n");
	tmp = lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_lst;
	lst->start = lst->i + 1;
}

// startからendまでの文字を文字をmallocして返す
char	*ms_create_word(char *s, int start, int end)
{
	char	*word;
	int		i;

	word = (char *)malloc(sizeof(char) * end - start + 1);
	if (!word)
		ms_error_exit("Malloc error\n");
	i = 0;
	while (start <= end)
	{
		word[i] = s[start];
		start++;
		i++;
	}
	return (word);
}

// 新しいリストを作る
t_word_list *ms_lst_new(char *s, int token_type, int detail_type)
{
	t_word_list	*new_lst;

	new_lst = (t_word_list *)malloc(sizeof(t_word_list));
	if (!new_lst)
		ms_error_exit("Malloc error\n");
	new_lst->next = NULL;
	new_lst->word = s;
	new_lst->detail_type = detail_type;
	new_lst->token_type = token_type;
	new_lst->i = 0;
	new_lst->start = 0;
	return (new_lst);
}
