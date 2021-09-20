/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_common.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 00:02:05 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/20 23:33:01 by yokawada         ###   ########.fr       */
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
}	t_master;

#endif
