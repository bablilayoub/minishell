/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:11:39 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/28 00:50:29 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

int	is_n_option(char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '-' && arg[1] == 'n')
	{
		i++;
		while (arg[i] == 'n')
			i++;
		if (arg[i] == '\0')
			return (1);
	}
	return (0);
}

int	is_space(char *arg)
{
	int	i;

	i = 0;
	if (!arg)
		return (1);
	if (arg[0] == '\0')
		return (0);
	while (arg[i] == ' ' || arg[i] == '\t')
		i++;
	if (arg[i] == '\0')
		return (1);
	return (0);
}

int	get_last_n_option(t_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd->arguments[i])
	{
		if (is_space(cmd->arguments[i]) || is_n_option(cmd->arguments[i]))
			i++;
		else
			break ;
	}
	if (cmd->arguments[i] == NULL)
		return (i);
	if (!is_space(cmd->arguments[i]) && !is_n_option(cmd->arguments[i]))
		i--;
	while (cmd->arguments[i] && is_space(cmd->arguments[i]))
		i--;
	if (cmd->arguments[i] && is_n_option(cmd->arguments[i]))
		i++;
	if (cmd->arguments[i] && is_space(cmd->arguments[i]))
		i++;
	return (i);
}

void	check_if_we_should_print_nl(t_cmd *cmd, int *should_print_nl)
{
	int	i;

	i = 1;
	while (cmd->arguments[i])
	{
		if (is_n_option(cmd->arguments[i]) || is_space(cmd->arguments[i]))
		{
			if (is_n_option(cmd->arguments[i]))
				*should_print_nl = 0;
		}
		else
			break ;
		i++;
	}
	if (*should_print_nl)
		i = 1;
	else
		i = get_last_n_option(cmd);
	while (cmd->arguments[i])
		printf("%s", cmd->arguments[i++]);
}

void	ft_echo(t_cmd *cmd, t_data *data)
{
	int	should_print_nl;

	if (!cmd || !cmd->arguments)
		return ;
	should_print_nl = 1;
	check_if_we_should_print_nl(cmd, &should_print_nl);
	if (should_print_nl)
		printf("\n");
	data->exit_status = 0;
}
