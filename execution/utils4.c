/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 01:56:46 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/17 01:57:09 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	get_file(t_cmd *cmd, char *file)
{
	int	fd;

	fd = 0;
	if (access(file, F_OK) == -1)
	{
		printf(PREFIX_ERROR "%s: no such file or directory\n",
			file);
		if (!cmd->built_in)
			exit(EXIT_FAILURE);
		return (-1);
	}
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		printf(PREFIX_ERROR "%s: %s\n", file, strerror(errno));
		if (!cmd->built_in)
			exit(EXIT_FAILURE);
		return (-1);
	}
	return (fd);
}
