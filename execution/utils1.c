/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:28:50 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/27 23:26:45 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	args_lenght(t_arg *args)
{
	t_arg	*tmp;
	int		i;

	i = 0;
	tmp = args;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

int	cmd_lenght(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void	close_fds(int **fd)
{
	int	i;

	i = -1;
	while (fd[++i])
	{
		if (write(fd[i][1], "", 0) == -1 && read(fd[i][0], "", 0) == -1)
			continue ;
		if (write(fd[i][1], "", 0) != -1)
		{
			if (close(fd[i][1]) == -1)
				perror("close");
		}
		if (read(fd[i][0], "", 0) != -1)
		{
			if (close(fd[i][0]) == -1)
				perror("close");
		}
	}
}

void	update_prefix(t_data *data, char *prefix)
{
	char	*new_prefix;

	if (!prefix || !prefix[0])
		prefix = "root";
	(1) && (free(data->prefix), new_prefix = ft_strjoin(RESET, BLUE));
	check_error_null(new_prefix, "malloc");
	(1) && (data->temp = new_prefix, new_prefix = ft_strjoin(new_prefix, BOLD));
	check_error_null(new_prefix, "malloc");
	(1) && (free(data->temp), data->temp = new_prefix);
	new_prefix = ft_strjoin(new_prefix, "➜  ");
	check_error_null(new_prefix, "malloc");
	free(data->temp);
	data->temp = new_prefix;
	new_prefix = ft_strjoin(new_prefix, prefix);
	check_error_null(new_prefix, "malloc");
	free(data->temp);
	data->temp = new_prefix;
	new_prefix = ft_strjoin(new_prefix, " : ");
	check_error_null(new_prefix, "malloc");
	free(data->temp);
	data->temp = new_prefix;
	new_prefix = ft_strjoin(new_prefix, RESET);
	check_error_null(new_prefix, "malloc");
	free(data->temp);
	data->prefix = new_prefix;
}

void	check_directory(char *str, char *dir, char **env)
{
	if (!dir || (dir[0] == '~' && dir[1] == '\0'))
	{
		chdir(ft_getenv("HOME", env));
		return ;
	}
	perror(str);
	chdir(dir);
	return ;
}
