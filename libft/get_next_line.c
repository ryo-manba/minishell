/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 14:54:45 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/05/31 08:02:31 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	find_newline(char *memo)
{
	int	i;

	i = 0;
	if (memo == NULL)
		return (-1);
	while (memo[i])
	{
		if (memo[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

static	int	get_split(char **memo, char **line, int newline_pos)
{
	char	*tmp;

	(*memo)[newline_pos] = '\0';
	*line = ft_strdup(*memo);
	tmp = ft_strdup(*memo + newline_pos + 1);
	safe_free(*memo);
	*memo = tmp;
	return (1);
}

static	int	check_memo(char **memo, char **line, int read_sz)
{
	int	newline_pos;

	if (read_sz < 0)
		return (-1);
	newline_pos = find_newline(*memo);
	if (*memo && newline_pos >= 0)
		return (get_split(memo, line, newline_pos));
	else if (*memo)
	{
		*line = *memo;
		*memo = NULL;
		return (0);
	}
	*line = ft_strdup("");
	return (0);
}

int	get_next_line(int fd, char **line)
{
	static char	*memo[MAX_FD];
	char		*buf;
	int			newline_pos;
	ssize_t		read_sz;

	if (fd < 0 || MAX_FD < fd || line == NULL || BUFFER_SIZE <= 0)
		return (-1);
	buf = (char *)malloc(sizeof(char) * (size_t)BUFFER_SIZE + 1);
	if (buf == NULL)
		return (-1);
	read_sz = read(fd, buf, BUFFER_SIZE);
	while (read_sz > 0)
	{
		buf[read_sz] = '\0';
		memo[fd] = free_strjoin(memo[fd], buf);
		newline_pos = find_newline(memo[fd]);
		if (newline_pos >= 0)
		{
			safe_free(buf);
			return (get_split(&memo[fd], line, newline_pos));
		}
		read_sz = read(fd, buf, BUFFER_SIZE);
	}
	safe_free(buf);
	return (check_memo(&memo[fd], line, read_sz));
}
