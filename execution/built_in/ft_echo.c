/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:11:39 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/15 00:03:46 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void write_rest_args(char **arguments)
{
	int i;

	i = -1;
	while (arguments[++i])
		printf("%s", arguments[i]);
}


bool is_all_n(char *arg)
{
	int i;

	i = 0;
	if (arg[i] == '-' && arg[i + 1] == 'n')
	{
		i++;
		while (arg[i])
		{
			if (arg[i] != 'n')
				return false;
			i++;
		}
	} 
	else
		return false;
	return true;
}

void ft_echo(t_cmd *cmd)
{
	t_cmd *tmp;
	int i;
	int valid;

	if (!cmd)
		return;
	tmp = cmd;
	valid = 0;
	if (tmp->arguments[1])
	{
		if (is_all_n(tmp->arguments[1]))
			valid = 1;
	}
	i = 0;
	while (tmp->arguments[++i])
	{
		if ((ft_strncmp(tmp->arguments[i], "-n", ft_strlen(tmp->arguments[i])) == 0) || (ft_strncmp(tmp->arguments[i], "\t", ft_strlen(tmp->arguments[i])) == 0)
			|| (ft_strncmp(tmp->arguments[i], " ", ft_strlen(tmp->arguments[i])) == 0) || is_all_n(tmp->arguments[i]))
		{
			if (is_all_n(tmp->arguments[i]))
				valid = 1;
			continue;
		}
		write_rest_args(tmp->arguments + i);
		break;
	}
	if (!valid)
		printf("\n");
	if (!cmd->next || !cmd->prev)
		return;  
}