/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 21:51:38 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/03 02:42:57 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void exit_shell(t_data *data)
{
	int fd_in;
	int bytes;
	char *env;
	char *buff;

	env = ft_strdup("");
	buff = malloc(sizeof(char) * 1024);
	fd_in = open(ft_strjoin(data->shell_path, "/exit.txt"), O_RDONLY);
	bytes = 1;
	while (bytes != 0)
	{
		bytes = read(fd_in, buff, 1024);
		if (bytes > 0)
		{
			buff[bytes] = '\0';
			env = ft_strjoin(env, buff);
		}
		else
			break;
	}
	if (!ft_strncmp(env, "exit", 4))
	{
		unlink(ft_strjoin(data->shell_path, "/exit.txt"));
		exit(130);
	}
	else
	{
		unlink(ft_strjoin(data->shell_path, "/exit.txt"));
		exit(atoi(env));
	}
}