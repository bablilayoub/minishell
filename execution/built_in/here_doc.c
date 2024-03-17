/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 03:51:23 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/16 04:51:42 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void	here_doc_signal(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		exit(EXIT_SUCCESS);
	}
}

int	here_doc(char *file_and_search_for)
{
	int		fd[2];
	char	*line;

	pipe(fd);
	signal(SIGINT, here_doc_signal);
	while (1)
	{
		line = readline(YELLOW "> " RESET);
		if (!line)
			break ;
		if (ft_strncmp(line, file_and_search_for, ft_strlen(line)) == 0)
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	return (fd[0]);
}
