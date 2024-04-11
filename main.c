/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:00:51 by abablil           #+#    #+#             */
/*   Updated: 2024/04/11 18:44:37 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing/parser.h"

int		g_child_open = 0;

int	main(int total, char **args, char **env)
{
	t_data	data;

	(void)args;
	if (total != 1)
	{
		printf(PREFIX_ERROR"Error: program takes no arguments\n");
		return (1);
	}
	data.empty_env = 0;
	data.temp2 = getcwd(NULL, 0);
	data.env = allocate_env(env, &data);
	data.export = allocate_export(env, &data);
	free(data.temp2);
	(1) && (data.token = NULL, data.cmd = NULL, data.in_valid = NULL,
	data.exit_status = 0, data.prefix = ft_strdup(PREFIX), data.temp = NULL,
	data.shell_path = getcwd(NULL, 0), rl_catch_signals = 0);
	data.found = 0;
	data.plus = 0;
	data.chk = NULL;
	get_parent_pid(&data);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	reader(&data);
	printf("exit\n");
	return (0);
}
