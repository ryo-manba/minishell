#ifndef MS_LEXER_H
# define MS_LEXER_H

# include "../libft/libft.h"
#include <stdio.h>

# define LC_NEWLINE	'N'
# define LC_SINGLE_QUOTE	'\''
# define LC_DOUBLE_QUOTE	'"'
# define LC_SPACE	' '
# define LC_TAB	'\t'
# define LC_PIPE	'|'
# define LC_AND	'&'
# define LC_REDIRECT_INPUT	'<'
# define LC_REDIRECT_OUTPUT	'>'
# define LC_SEMICOLON	';'
# define LC_PAREN_L	'('
# define LC_PAREN_R	')'
# define LC_WORD	'_'

typedef enum e_lex_type
{
	LT_DUMMY,
	LT_TOKEN,
	LT_IO_NUMBER,
	LT_OPERATOR,
	LT_NEWLINE,
}	t_lex_type;

typedef struct s_wdlist
{
	struct s_wdlist	*next;
	const char		*word;
	int				i;
	int				len;
	int				concluded;
	char			starting_chartype;
	char			delimiter;
	t_lex_type		lex_type;
}	t_wdlist;

typedef struct s_lex_cursor
{
	t_wdlist	*head;
	t_wdlist	*tail;
	const char	*line;
	char		under_quoted;
	int			i;
	int			failed;
}	t_lex_cursor;

# define CHARS_WORD_INCLUDED "\"'_"

t_wdlist	*ms_lexer(const char *line);
int			lx_add_token(t_lex_cursor *cursor, char starting_char);
void		lx_conclude_token(t_lex_cursor *cursor);
int			lx_is_digital_str(const	char *str, size_t len);
int			lx_is_char_for_name(char c, size_t pos);
int			lx_is_name(const char *str, size_t len);
int			lx_is_char_for_bare(char c);
int			lx_is_assignment_word(const char *str, size_t len);
void		lx_destroy_token(t_wdlist *t);

#endif
