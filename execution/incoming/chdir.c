/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:38:29 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/03 01:53:01 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

char **update_envpwd(char **env, char *oldpwd, char *pwd, char *path)
{
	// char	*temp;
	int i;

	i = -1;
	while (env[++i])
	{
		// temp = env[i];
		if (ft_strncmp(env[i], "OLDPWD", 6) == 0)
			env[i] = ft_strjoin("OLDPWD=", oldpwd);
		else if (ft_strncmp(env[i], "PWD", 3) == 0)
			env[i] = ft_strjoin("PWD=", pwd);
		else
			env[i] = ft_strdup(env[i]);
		// free(temp);
		if (!env[i])
		{
			free_double(env);
			free_triplet(oldpwd, pwd, path);
			exit(EXIT_FAILURE);
		}
	}
	env[i] = NULL;
	return (env);
}
void change_path(t_data *data)
{
	char *oldpwd;
	char *pwd;
	int fd_in;
	char *path;
	char *new_prefix;

    char *cd_path = ft_strjoin(data->shell_path, "/cd.txt");
	fd_in = open(cd_path, O_RDONLY, 0777);
	if (fd_in != -1)
	{
		check_error(fd_in, "open", 1);
		path = malloc(sizeof(char) * 1024);
		if (!path)
			return check_error_null(path, "malloc", 1);
		int bytes = read(fd_in, path, sizeof(char) * 1024);
		check_error(bytes, "read", 1);
		path[bytes] = '\0';
		oldpwd = getcwd(NULL, 0);
		if (!oldpwd)
			return check_error_null(oldpwd, "getcwd", 1);
		check_error(chdir(path), "chdir", 1);
		pwd = getcwd(NULL, 0);
		if (!pwd)
			return check_error_null(pwd, "getcwd", 1);
		data->env = update_envpwd(data->env, oldpwd, pwd, path);
		new_prefix = ft_strrchr(pwd, '/');
		update_prefix(data, new_prefix + 1);
		free_triplet(oldpwd, pwd, path);
		unlink(cd_path);
		close(fd_in);
	}
}