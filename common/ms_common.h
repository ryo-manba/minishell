/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_common.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 00:02:05 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/27 09:26:45 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_COMMON_H
# define MS_COMMON_H
# include <unistd.h>
# include <stdlib.h>
# ifndef MS_DEBUG
#  define MS_DEBUG 0
# endif
# define MS_DEBUG_PRINT_PARSED	0x1
# define MS_DEBUG_LEAKS	0x2

typedef struct s_splitcursor
{
	char	*c_line;
	char	**lines;
	size_t	i;
}	t_splitcursor;

typedef struct s_shellvar
{
	char				*key;
	char				*value;
	int					is_env;
	int					attr;
	struct s_shellvar	*next;
}	t_shellvar;

typedef enum e_token_id
{
	TI_DUMMY,
	TI_WORD,
	TI_IO_NUMBER,
	TI_NAME,
	TI_ASSIGNMENT_WORD,
	TI_SUBSHELL,
	TI_LT,
	TI_GT,
	TI_LTLT,
	TI_GTGT,
	TI_LTGT,
	TI_LTAND,
	TI_GTAND,
	TI_LTLTHYPHEN,
	TI_PIPE,
	TI_ANDAND,
	TI_PIPEPIPE,
	TI_AND,
	TI_SEMICOLON,
	TI_DSEMICOLON,
	TI_PAREN_L,
	TI_PAREN_R,
	TI_NONE,
}	t_token_id;

typedef struct s_op
{
	char		*label;
	t_token_id	token_id;
	size_t		len;
}	t_op;

typedef struct s_master
{
	char			*staring_prog_name;
	char			*prog_name;
	int				args_given;
	int				stdin_isatty;
	int				stderr_isatty;
	int				opt_c;
	int				opt_i;
	t_splitcursor	spcursor;
	int				interactive_shell;
	char			*filepath;
	t_shellvar		*var;
	size_t			line_num;
	char			*current_line;
	int				failed;
	char			*pwd;
	char			*old_pwd;
	int				exited;
	t_op			*lx_ops;
	t_op			*lx_all_ops;
}	t_master;

#endif
