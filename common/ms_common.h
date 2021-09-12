/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_common.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokawada <yokawada@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 00:02:05 by yokawada          #+#    #+#             */
/*   Updated: 2021/09/11 01:22:59 by yokawada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_COMMON_H
# define MS_COMMON_H
# include <unistd.h>
# include <stdlib.h>

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
	int			args_given;
	int			stdin_isatty;
	int			stderr_isatty;
	int			opt_c;
	char		*c_line;
	int			interactive_shell;
	char		*filepath;
	t_shellvar	*var;
	size_t		line_num;
	char		*current_line;
}	t_master;

#endif