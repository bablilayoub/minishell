/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:00:51 by abablil           #+#    #+#             */
/*   Updated: 2024/02/28 16:21:37 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **allocate(char **env)
{
	int i;
	char **new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
	{
		printf(PREFIX_ERROR"Error: failed to allocate memory\n");
		exit(1);
	}
	i = -1;
	while (env[++i])
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			printf(PREFIX_ERROR"Error: failed to allocate memory\n");
			exit(1);
		}
	}
	new_env[i] = NULL;
	return (new_env);
}
int main(int total, char **args, char **env)
{
	t_data data;
	(void)args;

	if (total != 1)
	{
		printf(PREFIX_ERROR"Error: program takes no arguments\n");
		return (1);
	}
	
	data.env = allocate(env);
	data.token = NULL;
	data.cmd = NULL;
	data.exit_status = 0;
	data.prefix = ft_strdup(PREFIX);
	reader(&data);
	return (0);
}
