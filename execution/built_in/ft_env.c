/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:14:02 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/31 18:20:03 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

bool	check_if_path_changed(char **env)
{
	int		i;
	bool	not_changed;

	i = -1;
	not_changed = 0;
	while (env[++i])
	{
		if (ft_strncmp(env[i],
				"PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.", 47) == 0)
			not_changed = 1;
	}
	return (not_changed);
}

bool	print_error_exit(t_data *data)
{
	printf(PREFIX_ERROR "env: No such file or directory\n");
	data->exit_status = 127;
	return (false);
}

void	ft_put_double_str(char **str, t_data *data)
{
	int		i;
	bool	not_changed;

	i = -1;
	not_changed = check_if_path_changed(str);
	while (str[++i])
	{
		if (data->empty_env)
		{
			if ((ft_strncmp(str[i], "PATH", 4) == 0 && not_changed)
				|| ft_strncmp(str[i], "SHELL", 4) == 0)
				continue ;
			printf("%s\n", str[i]);
		}
		else
			printf("%s\n", str[i]);
	}
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
	ft_put_double_str(env, data);
	free_array(path);
	return (true);
}
