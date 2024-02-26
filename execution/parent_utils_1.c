/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 21:51:38 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/26 21:52:41 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void update_env(t_data *data)
{
	int fd_in;
	int bytes;
	char *env;
	char *buff;

	env = ft_strdup("");
	buff = malloc(sizeof(char) * 1024);
	fd_in = open("export.txt", O_RDONLY);
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
	unlink("export.txt");
	data->env = ft_split(env, '\n');
	close(fd_in);
}
char **update_envpwd(char **env, char *oldpwd, char *pwd)
{
	int i;
	char **new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], "OLDPWD", 6) == 0)
			new_env[i] = ft_strjoin("OLDPWD=", oldpwd);
		else if (ft_strncmp(env[i], "PWD", 3) == 0)
			new_env[i] = ft_strjoin("PWD=", pwd);
		else
			new_env[i] = ft_strdup(env[i]);
	}
	new_env[i] = NULL;
	return (new_env);
}
void change_path(t_data *data, char *path)
{
	char *oldpwd;
	char *pwd;
	int fd_in;

	(void)data;
	fd_in = open("cd.txt", O_RDONLY);
	path = malloc(sizeof(char) * 1024);
	read(fd_in, path, 1024);
	close(fd_in);
	unlink("cd.txt");
	oldpwd = getcwd(NULL, 0);
	chdir(path);
	pwd = getcwd(NULL, 0);
	data->env = update_envpwd(data->env, oldpwd, pwd);
	char *new_prefix = ft_strrchr(pwd, '/');
	update_prefix(data, new_prefix + 1);
	// free(oldpwd);
	// free(pwd);
	// free(path);
}

void unset_env(t_data *data)
{
	int fd_in;
	int bytes;
	char *env;
	char *buff;

	env = ft_strdup("");
	buff = malloc(sizeof(char) * 1024);
	fd_in = open("unset.txt", O_RDONLY);
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
	unlink("unset.txt");
	data->env = ft_split(env, '\n');
	close(fd_in);
}

void exit_shell(void)
{
	int fd_in;
	int bytes;
	char *env;
	char *buff;

	env = ft_strdup("");
	buff = malloc(sizeof(char) * 1024);
	fd_in = open("exit.txt", O_RDONLY);
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
		unlink("exit.txt");
		exit(130);
	}
	else
	{
		unlink("exit.txt");
		exit(atoi(env));
	}
}