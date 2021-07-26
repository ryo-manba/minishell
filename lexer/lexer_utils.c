#include "lexer.h"

int	sta_get_size(t_stack *sta)
{
	int	len;
	t_stack *tmp;

	len = 0;
	tmp = sta;
	while (tmp)
	{
		tmp = tmp->next;
		len++;
	}
	return (len);
}

t_stack	*stack_new(char c)
{
	t_stack	*new_sta;

	new_sta = (t_stack *)malloc(sizeof(t_stack));
	if (!new_sta)
		return (NULL);
	new_sta->next = NULL;
	new_sta->c = c;
	new_sta->i = 0;
	new_sta->start = 0;
	return (new_sta);
}

bool	sta_push_back(t_stack *sta, char c)
{
	t_stack	*new_sta;
	t_stack	*tmp;

	new_sta = stack_new(c);
	if (!new_sta)
		return (false);
	tmp = sta;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_sta;
	return (true);
}

char	sta_pop_front(t_stack **sta)
{
	t_stack	*tmp;
	char	c;

	c = (*sta)->c;
	tmp = (*sta)->next;
	free(*sta);
	*sta = tmp;
	return (c);
}

int	check_word(char c)
{
	if (c == '\'')
		return (TYPE_SQUOTE);
	if (c == '\"')
		return (TYPE_DQUOTE);
	if (c == ' ')
		return (TYPE_SPACE);
	if (c == '$')
		return (TYPE_DOLLAR);
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

char	*create_word(t_stack *sta, int start, int end)
{
	char	*s;
	t_stack	*tmp;
	int		i;
	int		j;

	s = (char *)malloc(sizeof(char) * end - start + 1);
	if (!s)
		return (NULL);
	i = 0;
	j = 0;
	tmp = sta;
	while (j < start)
	{
		tmp = tmp->next;
		j++;
	}
	while (start <= end)
	{
		s[i] = tmp->c;
		tmp = tmp->next;
		start++;
		i++;
	}
//	printf("%s, %s\n", s, __func__);
	return (s);
}

t_word_list *wordlst_new(char *s, int flag)
{
	t_word_list	*new_lst;

	new_lst = (t_word_list *)malloc(sizeof(t_word_list));
	if (!new_lst)
		return (NULL);
	new_lst->next = NULL;
	new_lst->word = s;
	new_lst->flags = flag;
	return (new_lst);
}

bool	lst_push_back(t_word_list *lst, char *s, int flag)
{
	t_word_list	*new_lst;
	t_word_list	*tmp;

	new_lst = wordlst_new(s, flag);
	if (!new_lst)
		return (false);
	tmp = lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_lst;
	return (true);
}
