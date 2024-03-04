/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:00:51 by abablil           #+#    #+#             */
/*   Updated: 2024/03/04 18:11:09 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing/parser.h"

int	main(int total, char **args, char **env)
{
	t_data	data;

	(void)args;
	if (total != 1)
	{
		printf(PREFIX_ERROR"Error: program takes no arguments\n");
		return (1);
	}
	data.env = allocate_env(env);
	data.token = NULL;
	data.cmd = NULL;
	data.exit_status = 0;
	data.prefix = ft_strdup(PREFIX);
	data.shell_path = getcwd(NULL, 0);
	reader(&data);
	return (0);
}
