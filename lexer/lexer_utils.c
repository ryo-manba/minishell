#include "lexer.h"

int	check_word(char c)
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
		return (TYPE_AND); // bonus
	if (c == '*')
		return (TYPE_WILD); // bonus
	if (c == '<')
		return (REDIRECT_INPUT);
	if (c == '>')
		return (REDIRECT_OUTPUT);
	return (TYPE_NORMAL);
}

int	check_type(int type)
{
	if (type == REDIRECT_INPUT)
		type = TYPE_HEREDOC;
	if (type == REDIRECT_OUTPUT)
		type = REDIRECT_APPEND;
	if (type == TYPE_PIPE)
		type = TYPE_DOUBLE_PIPE;
	if (type == TYPE_AND)
		type = TYPE_DOUBLE_AND;
	return (type);
}

char	*create_word(char *s, int start, int end)
{
	char	*word;
	int		i;

	word = (char *)malloc(sizeof(char) * end - start + 2);
	if (!word)
		return (NULL);
	i = 0;
	while (start <= end)
	{
		word[i] = s[start];
		start++;
		i++;
	}
	word[i] = '\0';
	return (word);
}

t_word_list *lst_new(char *s, int token_type, int detail_type)
{
	t_word_list	*new_lst;

	new_lst = (t_word_list *)malloc(sizeof(t_word_list));
	if (!new_lst)
		return (NULL);
	new_lst->next = NULL;
	new_lst->word = s;
	new_lst->detail_type = detail_type;
	new_lst->token_type = token_type;
	new_lst->i = 0;
	new_lst->start = 0;
	return (new_lst);
}

bool	lst_push_back(t_word_list *lst, char *s, int token_type, int detail_type)
{
	t_word_list	*new_lst;
	t_word_list	*tmp;

	new_lst = lst_new(s, token_type, detail_type);
	if (!new_lst)
		return (false);
	tmp = lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_lst;
	return (true);
}
