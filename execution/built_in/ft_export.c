/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 14:46:52 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/22 17:56:58 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

int	ft_is_alphanumeric(char *key)
{
	int	i;

	i = -1;
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (false);
	}
	return (true);
}

void	load_to_exp(char ***export, char *args, char *key_arg, int *found)
{
	char	*sub;
	char	**key;
	char	*temp;
	int		j;

	j = -1;
	while ((*export)[++j])
	{
		sub = ft_substr((*export)[j], 11, ft_strlen((*export)[j]));
		check_error_null(sub, "malloc");
		key = ft_split(sub, '=');
		check_error_null(key, "malloc");
		if (ft_strncmp(key_arg, key[0], ft_strlen(key[0])) == 0)
		{
			if (ft_strchr(args, '='))
			{
				temp = (*export)[j];
				(*export)[j] = ft_strjoin("declare -x ", args);
				check_error_null((*export)[j], "malloc");
				free(temp);
			}
			*found = 1;
		}
		(1) && (free_array(key), free(sub), key = NULL);
	}
}

void	export_to_exp(char ***export, char **args, t_data *data)
{
	int		i;
	int		found;
	char	**key_arg;

	i = 0;
	while (args[++i])
	{
		found = 0;
		if (!check_exported(args[i], 0, data))
			continue ;
		key_arg = ft_split(args[i], '=');
		if (!key_arg)
			exit(EXIT_FAILURE);
		load_to_exp(export, args[i], key_arg[0], &found);
		if (!found)
			add_var(export, ft_strjoin("declare -x ", args[i]), 1);
		free_double(key_arg);
	}
}

void	export_to_env(char ***env, char **key_val, t_data *data, int *found)
{
	int		i;
	char	*check;

	i = -1;
	check = key_val[0];
	while ((*env)[++i])
	{
		if (ft_strchr(key_val[0], '+'))
			key_val[0] = ft_substr(key_val[0], 0, (ft_strlen(key_val[0]) - 1));
		printf("%s\n", key_val[0]);
		if (ft_strncmp((*env)[i], key_val[0], ft_strlen(key_val[0]) == 0))
		{
			data->temp = (*env)[i];
			if (ft_strchr(check, '+'))
			{
				(*env)[i] = ft_strjoin((*env)[i], key_val[1]);
					// printf("%s\n", (*env)[i]);
			}
			else
			{
				(*env)[i] = ft_strjoin(key_val[0], "=");	
				check_error_null((*env)[i], "malloc");
				(1) && (free(data->temp), data->temp = (*env)[i]);				
				(*env)[i] = ft_strjoin((*env)[i], key_val[1]);				
				check_error_null((*env)[i], "malloc");
			}
			(1) && (free(data->temp), *found = 1);
		}
	}
}

void	ft_export(t_data *data, char ***env)
{
	char	**key_val;
	int		found;
	int		j;

	key_val = NULL;
	j = 0;
	while (data->cmd->arguments[++j])
	{
		if (!check_exported(data->cmd->arguments[j], 1, data))
			continue ;
		key_val = key_value(data->cmd->arguments[j]);
		if (!key_val)
			exit(EXIT_FAILURE);
		found = 0;
		export_to_env(env, key_val, data, &found);
		free_double(key_val);
		if (!found)
			add_var(env, data->cmd->arguments[j], 0);
	}
	export_to_exp(&data->export, data->cmd->arguments, data);
	if (!data->cmd->next || !data->cmd->prev)
		return ;
}
