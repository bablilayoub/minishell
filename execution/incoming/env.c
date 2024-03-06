/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:18:24 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/06 01:48:08 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

int set_fd(t_data *data, int *fd_in, char *cmd)
{
	if (ft_strncmp(cmd, "export", 6) == 0)
		*fd_in = open(ft_strjoin(data->shell_path, "/export.txt"), O_RDONLY);
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
		result = unlink(ft_strjoin(data->shell_path, "/export.txt"));
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

void update_export(char ***export, char **tmp)
{
	char **temp;
	int i;
	int j;

	i = 0;
	temp = *export;
	*export = malloc(sizeof(char *) * (ft_strdoublelen(*export) + ft_strdoublelen(tmp) + 1));
	if (!*export)
		exit(EXIT_FAILURE);
	while (temp[i])
	{
		(*export)[i] = temp[i];
		i++;
	}
	j = 0;
	while (tmp[j])
	{
		(*export)[i] = tmp[j];
		i++;
		j++;
	}
	(*export)[i] = NULL;
}
void extract_exported(char **env, char ***export, char **args)
{
	int i;
	int j;
	int k;
	char **tmp;
	static int argc = 0;

	i = 0;
	while (args[++i] != NULL)
			argc++;
	printf("argc = %d\n", argc);
	tmp = malloc(sizeof(char *) * (argc + 1));
	i = 0;
	k = 0;
	while (args[++i])
	{
		j = -1;
		if (!ft_strchr(args[i], '='))
		{
			tmp[k++] = ft_strjoin("declare -x ", args[i]);
			continue;
		}
		while (env[++j])
		{
			if (ft_strncmp(env[j], args[i], ft_strlen(args[i])) == 0)
				tmp[k++] = ft_strjoin("declare -x ", env[j]);
		}
	}
	tmp[k] = NULL;
	if (!*export)
		*export = tmp;
	else
		update_export(export, tmp);
}
void print(char **export)
{
	int i;

	if (!export)
		return;
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
		return;
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