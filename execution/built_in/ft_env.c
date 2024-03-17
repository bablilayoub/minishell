/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:14:02 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/16 04:51:09 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

bool	print_error_exit(t_data *data)
{
	printf(PREFIX_ERROR "env: No such file or directory\n");
	data->exit_status = 127;
	return (false);
}

bool	ft_env(char **env, t_data *data)
{
	int		i;
	char	**path;
	int		found;

	if (!env)
		return (false);
	path = ft_split(ft_getenv("PATH", env), ':');
	if (!path)
		return (print_error_exit(data));
	(1) && (i = -1, found = 0);
	while (path[++i])
	{
		if (ft_strncmp(path[i], "/usr/bin", ft_strlen(path[i])) == 0)
			found = 1;
	}
	if (!found)
	{
		free_array(path);
		return (failure("env", data));
	}
	i = -1;
	while (env[++i])
		printf("%s\n", env[i]);
	free_array(path);
	return (true);
}
