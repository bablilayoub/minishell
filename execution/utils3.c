/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 04:43:53 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/25 15:29:07 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	check_error(int fd, const char *msg, int flag)
{
	if (fd == -1)
	{
		perror(msg);
		if (flag)
			return ;
		else
			exit(EXIT_FAILURE);
	}
}

void	check_error_null(void *ptr, const char *msg)
{
	if (!ptr)
	{
		printf(PREFIX_ERROR "%s : %s\n", msg, strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void	free_triplet(char *s1, char *s2, char *s3)
{
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	if (s3)
		free(s3);
}

void	free_int(int **fd)
{
	int	i;

	i = -1;
	while (fd[++i])
		free(fd[i]);
	free(fd);
}

bool	failure(char *cmd, t_data *data)
{
	printf(PREFIX_ERROR "command not found: %s\n", cmd);
	data->exit_status = 127;
	return (false);
}
