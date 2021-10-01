/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_fd_write.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 12:35:51 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/10/01 12:37:44 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"

int	main(int argc, char **argv)
{
	write(ft_atoi(argv[1]), argv[1], ft_strlen(argv[1]));
	write(ft_atoi(argv[1]), "\n", 1);
	return (0);
}
