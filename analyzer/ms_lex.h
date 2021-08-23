#ifndef MS_LEX_H
# define MS_LEX_H

# include "../libft/libft.h"
# include <stdio.h>

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

typedef	enum e_lex_type
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
	char			*word;
	int				i;
	int				len;
	int				concluded;
	char			starting_chartype;
	char			right_delimiter;
	t_lex_type		lex_type;
}	t_wdlist;

typedef struct s_lex_cursor
{
	t_wdlist	*head;
	t_wdlist	*tail;
	const char	*line;
	char		under_quoted;
	int			i;
}	t_lex_cursor;

# define UQ_SINGLE 1
# define UQ_DOUBLE 2

# define CHARS_WORD_INCLUDED "\"'_"

void		ms_add_lexer_token(t_lex_cursor *cursor, char starting_char);
void		ms_conclude_lexer_token(t_lex_cursor *cursor);
int			ms_is_digital_str(const	char *str, size_t len);
int			ms_is_name(const char *str, size_t len);
int			ms_is_assignment_word(const char *str, size_t len);
t_wdlist	*ms_lexer(const char *line);

#endif
