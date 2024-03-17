/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:00:51 by abablil           #+#    #+#             */
/*   Updated: 2024/03/17 03:53:32 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing/parser.h"

void	f(void)
{
	system("leaks minishell");
}

int	main(int total, char **args, char **env)
{
	t_data	data;

	printf("PID: %d\n", getpid());
	atexit(f);
	(void)args;
	if (total != 1)
	{
		printf(PREFIX_ERROR"Error: program takes no arguments\n");
		return (1);
	}
	data.env = allocate_env(env);
	data.export = allocate_export(env);
	data.token = NULL;
	data.cmd = NULL;
	data.in_valid = NULL;
	data.exit_status = 0;
	data.prefix = ft_strdup(PREFIX);
	data.shell_path = getcwd(NULL, 0);
	data.temp = NULL;
	reader(&data);
	return (0);
}
