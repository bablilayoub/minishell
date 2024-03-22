/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 04:41:00 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/22 14:05:55 by abablil          ###   ########.fr       */
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
	if (oldpwd)
		free(oldpwd);
	if (cmd->next || cmd->prev)
		exit(EXIT_FAILURE);
	else
		return ;
}

void	print(char **export)
{
	int		i;
	int		k;
	bool	swapped;
	char	*temp;

	if (!export || !*export)
		return ;
	k = 11;
	swapped = true;
	while (swapped)
	{
		swapped = false;
		i = 0;
		while (export[i] && export[i + 1])
		{
			if (export[i][k] > export[i + 1][k])
			{
				temp = export[i];
				export[i] = export[i + 1];
				export[i + 1] = temp;
				swapped = true;
			}
			i++;
		}
	}	
	i = -1;
	while (export[++i])
		printf("%s\n", export[i]);
}
