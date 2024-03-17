/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 04:41:00 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/15 04:45:56 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	free_double(char **ptr)
{
	int	i;

	i = -1;
	while (ptr[++i])
	{
		if (ptr[i])
			free(ptr[i]);
	}
	if (ptr)
		free(ptr);
}

void	free_two(char *s1, char *s2)
{
	if (s1)
		free(s1);
	if (s2)
		free(s2);
}

int	ft_strdoublelen(char **str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	print_error(t_cmd *cmd, char *dirname, char *oldpwd)
{
	printf(PREFIX_ERROR "cd: %s: %s\n", dirname, strerror(errno));
	free(oldpwd);
	if (cmd->next || cmd->prev)
		exit(EXIT_FAILURE);
	else
		return ;
}

void	print(char **export)
{
	int	i;

	if (!export || !*export)
		return ;
	i = -1;
	while (export[++i])
		printf("%s\n", export[i]);
}
