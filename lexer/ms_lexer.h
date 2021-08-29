#ifndef MS_LEXER_H
# define MS_LEXER_H
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <readline/history.h>
# include <readline/readline.h>

typedef struct s_word_list
{
	struct s_word_list	*next;
	char	*word;
	int		detail_type;
	int		token_type;
	int		i;
	int		start;
}	t_word_list;

typedef enum s_status
{
	TYPE_NORMAL,
	TYPE_SPACE,
	TYPE_SINGLE_QUOTE,
	TYPE_DOUBLE_QUOTE,
	TYPE_AND,
	TYPE_PIPE,
	TYPE_WILD,
	TYPE_DOUBLE_AND,
	TYPE_DOUBLE_PIPE,
	TYPE_HEREDOC,
	REDIRECT_INPUT,
	REDIRECT_OUTPUT,
	REDIRECT_APPEND,
}	t_status;

typedef enum s_token_type
{
	RESERVED, // 予約語いらない
	WORD,
	OPERATOR,
}	t_token_type;

/* ms_lexer */
t_word_list *ms_lexer(char *s);
bool		ms_check_open_quote_and_delimiter(t_word_list *lst, char *s);
void 		ms_update_lst_accordingly_type(t_word_list *lst, char *s, int type);
void		ms_in_quoting(t_word_list *lst, char *s, int type);

/* ms_lexer_utils */
void		ms_error_exit(char *error_message);
void 		ms_free_head_lst(t_word_list **lst);
void		ms_lst_push_back(t_word_list *lst, char *s, int token_type, int detail_type);
char		*ms_create_word(char *s, int start, int end);
t_word_list *ms_lst_new(char *s, int token_type, int detail_type);

/* ms_lexer_checker */
bool		ms_check_delimiter(char c);
int			ms_check_word_type(char c);
int			ms_check_consecutively_operator(int type);
void		ms_check_operator_and_update_lst(t_word_list *lst, char *s, int type);
void		ms_check_consecutively_and_update_lst(t_word_list *lst, char *s, int type);

#endif
