/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmatsuka <rmatsuka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 19:09:03 by rmatsuka          #+#    #+#             */
/*   Updated: 2021/09/23 21:53:50 by rmatsuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"

// Close the connected pipe and keep the previous pipe.
void	ms_close_and_update_pipe(int pipe_fd[2], int before_pipe[2])
{
	if (before_pipe[0] != -1 && before_pipe[1] != -1)
	{
		if (close(before_pipe[0]) == -1 || \
			close(before_pipe[1]) == -1)
			ms_perror_exit("close");
	}
	before_pipe[0] = pipe_fd[0];
	before_pipe[1] = pipe_fd[1];
}

void	ms_do_piping(t_clause *clause, int pipe_fd[2], int before_pipe[2])
{
	if (before_pipe[0] == -1)
		ms_first_pipe(pipe_fd);
	else if (clause->next == NULL)
		ms_last_pipe(before_pipe);
	else
		ms_middle_pipe(pipe_fd, before_pipe);
}

// First command
// Connect standard output to the pipe entrance.
void	ms_first_pipe(int pipe_fd[2])
{
	if (close(pipe_fd[0]) == -1)
		ms_perror_exit("close");
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		ms_perror_exit("dup2");
	if (close(pipe_fd[1]) == -1)
		ms_perror_exit("close");
}

// Last command
// Output remains the same, only input is taken from the previous pipe.
void	ms_last_pipe(int before_pipe[2])
{
	if (close(before_pipe[1]) == -1)
		ms_perror_exit("close");
	if (dup2(before_pipe[0], STDIN_FILENO) == -1)
		ms_perror_exit("dup2");
	if (close(before_pipe[0]) == -1)
		ms_perror_exit("close");
}

// The command is on the way, so do both of the above processes.
void	ms_middle_pipe(int pipe_fd[2], int before_pipe[2])
{
	ms_last_pipe(before_pipe);
	ms_first_pipe(pipe_fd);
}
