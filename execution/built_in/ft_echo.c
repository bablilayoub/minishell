/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:11:39 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/20 00:44:34 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void	write_rest_args(char **arguments)
{
	int	i;

	i = -1;
	while (arguments[++i])
		printf("%s", arguments[i]);
}

bool	is_all_n(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' && arg[i + 1] == 'n')
	{
		i++;
		while (arg[i])
		{
			if (arg[i] != 'n')
				return (false);
			i++;
		}
	}
	else
		return (false);
	return (true);
}

void	print_arguments(char **arguments, int *valid)
{
	int	i;

	i = 0;
	while (arguments[++i])
	{
		if ((ft_strncmp(arguments[i], "-n", ft_strlen(arguments[i])) == 0)
			|| (ft_strncmp(arguments[i], "\t", ft_strlen(arguments[i])) == 0)
			|| (ft_strncmp(arguments[i], " ", ft_strlen(arguments[i])) == 0)
			|| is_all_n(arguments[i]))
		{
			if (is_all_n(arguments[i]))
				*valid = 1;
			continue ;
		}
		write_rest_args(arguments + i);
		break ;
	}
}

void	ft_echo(t_cmd *cmd, t_data *data)
{
	t_cmd	*tmp;
	int		valid;

	if (!cmd)
		return ;
	tmp = cmd;
	valid = 0;
	if (tmp->arguments[1])
	{
		if (is_all_n(tmp->arguments[1]))
			valid = 1;
	}
	print_arguments(tmp->arguments, &valid);
	if (!valid)
		printf("\n");
	data->exit_status = 0;
	if (!cmd->next || !cmd->prev)
		return ;
}
