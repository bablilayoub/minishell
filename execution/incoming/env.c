/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:18:24 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/07 22:24:18 by alaalalm         ###   ########.fr       */
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


void export_env_to_exp(char **env, char ***export)
{
	int i;
	int lenght;

	lenght = ft_strdoublelen(env);
	*export = (char **)malloc(sizeof(char *) * (lenght + 1));
	if (!export)
		exit(EXIT_FAILURE);
	i = -1;
	while (env[++i])
		(*export)[i] = ft_strjoin("declare -x ", env[i]);
	(*export)[i] = NULL;
}
void *ft_realloc(void *ptr, size_t size)
{
	void *new;

	new = malloc(size);
	if (!new)
		return NULL;
	if (ptr)
	{
		ft_memcpy(new, ptr, size);
		free(ptr);
	}
	return new;
}

int isDuplicate(char **arr, int n, char *str)
{
    int i = 0;
    while (i < n) 
	{
        if (ft_strncmp(arr[i], str, ft_strlen(str)) == 0)
            return 1;
        i++;
    }
    return 0;
}

void removeDuplicates(char ***invalid, int n)
{
    char **newArr = NULL;
    int count = 0;
    int i = 0;
	char **arr;
	
	arr = *invalid;
    while (i < n)
	{
        if (!isDuplicate(newArr, count, arr[i]))
		{
            newArr = realloc(newArr, (count + 1) * sizeof(char *));
            newArr[count] = ft_strdup(arr[i]);
            count++;
        }
        i++;
    }
	newArr = realloc(newArr, (count + 1) * sizeof(char *));
	newArr[count] = NULL;
	free_double(*invalid);
    *invalid = newArr;
}

void join_double(char ***join, char **tmp)
{
	int i;
	int j;
	char **new;

	if (!tmp)
		return ;
	i = ft_strdoublelen(*join);
	j = ft_strdoublelen(tmp);
	new = (char **)malloc(sizeof(char *) * (i + j + 1));
	if (!new)
		exit(EXIT_FAILURE);
	i = -1;
	while ((*join)[++i])
		new[i] = ft_strdup((*join)[i]);
	j = -1;    
	while (tmp[++j])
		new[i++] = ft_strdup(tmp[j]);
	new[i] = NULL;
	free_double(*join);
	*join = new;
}
void add_invalid_exp(char **args, t_data *data)
{
	int i;
	int k;
	char **tmp;
	int argc = 0;

	i = 0;
	while (args[++i])
	{
		if (!ft_strchr(args[i], '='))
			argc++;
	}
	tmp = malloc(sizeof(char *) * (argc + 1));
	i = 0;
	k = 0;
	while (args[++i])
	{
		if (!ft_strchr(args[i], '='))
			tmp[k++] = ft_strjoin("declare -x ", args[i]);
	}
	tmp[k] = NULL;
	if (!data->in_valid)
		data->in_valid = tmp;
	else
		join_double(&data->in_valid, tmp);
	removeDuplicates(&data->in_valid, ft_strdoublelen(data->in_valid));
}
void print(char **export)
{
	int i;

	if (!export || !*export)
		return;
	i = -1;
	while (export[++i])
		printf("%s\n", export[i]);
}
void update_env(t_data *data, char *cmd_name, char **args)
{        
	int fd_in;
	char *buff;
	char *env;

	(void)args;
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
	add_invalid_exp(args, data);
	free_two(env, buff);
	close(fd_in);
}