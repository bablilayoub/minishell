/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:18:24 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/03 02:48:08 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

int set_fd(t_data *data, int *fd_in, char *cmd)
{
	if (ft_strncmp(cmd, "export", 6) == 0)
		*fd_in = open("export.txt", O_RDONLY);
	else
		*fd_in = open(ft_strjoin(data->shell_path, "/unset.txt"), O_RDONLY);
	if (*fd_in == -1)
		return -1;
	return *fd_in;
}

int unlink_file_and_update(t_data *data, char *cmd, char ***to_update, char *env, char *buff)
{
	char	**temp;
	char    **envp;
	int		result;
	
	envp = *to_update;
	if (ft_strncmp(cmd, "export", 6) == 0)
		result = unlink("export.txt");
	else
		result = unlink(ft_strjoin(data->shell_path, "/unset.txt"));
	if (result == -1)
	{
		free_two(env, buff);
		return -1;
	}
	temp = envp;
	envp = ft_split(env, '\n');
	if (!envp)
	{
		free_two(env, buff);
		return -1;
	}
	free_double(temp);
	*to_update = envp;
	return 0;
}
int read_incoming(int *fd_in, char *buff, char **env)
{
	int bytes;
	char *temp;
	
	bytes = 1;
	while (bytes != 0)
	{
		temp = *env;	
		bytes = read(*fd_in, buff, 1024);
		if (bytes == -1)
			return -1;
		if (bytes > 0)
		{
			buff[bytes] = '\0';
			*env = ft_strjoin(*env, buff);
			free(temp);
			if (!*env)
				return -1;
		}
		else
			break;
	}
	return 0;
}

void update_env(t_data *data, char *cmd_name)
{
	int fd_in;
	char *buff;
	char *env;

	env = ft_strdup("");
	if (!env)
		return ;
	buff = malloc(sizeof(char) * 1024);
	if (!buff)
		return ;
	fd_in = set_fd(data, &fd_in, cmd_name);
	if (fd_in == -1)
	{
		free_two(env, buff);
		return check_error(fd_in, "open", 1);
	}
	if (read_incoming(&fd_in, buff, &env) == -1)
	{
		free_two(env, buff);
		close(fd_in);
		return ;
	}
	if (unlink_file_and_update(data, cmd_name, &data->env, env, buff) == -1)
	{
		free_two(env, buff);
		close(fd_in);
		return ;
	}
	free_two(env, buff);
	close(fd_in);
}