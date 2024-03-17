/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:14:21 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/16 03:12:48 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

int	is_valid_number(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	two_many_arguments(int *exit_status, t_cmd *cmd)
{
	*exit_status = 1;
	printf(PREFIX_ERROR "exit: too many arguments\n");
	if (cmd->next || cmd->prev)
		exit(1);
	else
		return ;
}

void	ft_exit(t_data *data, t_cmd *cmd)
{
	printf("exit\n");
	if (cmd->arguments[1] && !is_valid_number(cmd->arguments[1]))
	{
		data->exit_status = 255;
		printf(PREFIX_ERROR "%s: numeric argument required", cmd->arguments[1]);
		exit(255);
	}
	if (cmd->arguments[2] && cmd->arguments[1])
		two_many_arguments(&data->exit_status, cmd);
	if (!cmd->arguments[1])
		exit(0);
	else
	{
		if (cmd->arguments[1][0] == '-')
		{
			data->exit_status = 256 - ft_atoi(cmd->arguments[1] + 1);
			exit(256 - ft_atoi(cmd->arguments[1] + 1));
		}
		else
		{
			data->exit_status = ft_atoi(cmd->arguments[1]);
			exit(ft_atoi(cmd->arguments[1]));
		}
	}
}
