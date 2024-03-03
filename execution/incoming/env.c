/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:18:24 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/03 02:56:34 by alaalalm         ###   ########.fr       */
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
	char **temp;
	char **envp;
	int result;

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

void extract_exported(char **env, char ***export, char **args)
{
	int i;
	int j;
	char **tmp;
	int argc;
	static int extract = 0;

	extract += ft_strdoublelen(args) - 1;
	argc = extract;
	tmp = malloc(sizeof(char *) * (argc + 1));
	i = 0;
	j = 0;
	while (env[i])
	{
		if ((ft_strdoublelen(env) - extract) == i)
		{
			tmp[j] = ft_strdup("declare -x ");
			tmp[j] = ft_strjoin(tmp[j], env[i]);
			j++;
		}
		i++;
	}
	tmp[j] = NULL;
	*export = tmp;
}

void print(char **export)
{
	int i;

	i = 0;
	while (export[i])
	{
		printf("%s\n", export[i]);
		i++;
	}
}
void update_env(t_data *data, char *cmd_name, char **args)
{
	int fd_in;
	char *buff;
	char *env;

	env = ft_strdup("");
	if (!env)
		return;
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
		return;
	}
	if (unlink_file_and_update(data, cmd_name, &data->env, env, buff) == -1)
	{
		free_two(env, buff);
		close(fd_in);
		return;
	}
	extract_exported(data->env, &data->export, args);
	free_two(env, buff);
	close(fd_in);
}