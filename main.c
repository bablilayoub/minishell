/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:00:51 by abablil           #+#    #+#             */
/*   Updated: 2024/03/03 01:44:47 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing/parser.h"

char	**allocate(char **env)
{
	int		i;
	char	**new_env;

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

int	main(int total, char **args, char **env)
{
	t_data	data;

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
	data.shell_path = getcwd(NULL, 0);
	reader(&data);
	return (0);
}
