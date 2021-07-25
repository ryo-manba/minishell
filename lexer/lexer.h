#include <stdio.h>
#include <stdlib.h>
#include "./libft/libft.h"
typedef struct s_word_desc
{
	char	*word;
	int		flags;
}	t_word_desc;

typedef struct s_word_list
{
	struct s_word_list	*next;
	char	*word;
	int		flags;
}	t_word_list;

typedef	enum s_status
{
	TYPE_DOUBLE,
	TYPE_SINGLE,
	TYPE_NORMAL,
	TYPE_SPACE,
}	t_status;

typedef struct s_stack
{
	char	c;
	struct s_stack	*next;
}	t_stack;

int	sta_get_size(t_stack *sta);
t_stack	*stack_new(char c);
bool	sta_push_back(t_stack *sta, char c);
char	sta_pop_front(t_stack **sta);
char	*create_word(t_stack *sta, int start, int end);
int	check_word(char c);
t_word_list *wordlst_new(char *s, int flag);
bool	lst_push_back(t_word_list *lst, char *s, int flag);
