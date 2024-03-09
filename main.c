/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:00:51 by abablil           #+#    #+#             */
/*   Updated: 2024/03/09 21:27:05 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing/parser.h"


char **allocate_export(char **env)
{
	int i;

	int lenght;
	char **tmp;

	lenght = ft_strdoublelen(env);
	tmp = malloc(sizeof(char *) * (lenght + 1));
	if (!tmp)
		exit(EXIT_FAILURE);
	i = -1;
	while (env[++i])
		tmp[i] = ft_strjoin("declare -x ", env[i]);
	tmp[i] = NULL;
	return (tmp);
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
	data.env = allocate_env(env);
	data.export = allocate_export(data.env);
	data.token = NULL;
	data.cmd = NULL;
	data.in_valid = NULL;
	data.exit_status = 0;
	data.prefix = ft_strdup(PREFIX);
	data.shell_path = getcwd(NULL, 0);
	reader(&data);
	return (0);
}
