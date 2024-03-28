/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 22:32:35 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/28 05:57:03 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

int	ft_is_alphanumeric(char *key, char *exported)
{
	int		i;

	i = -1;
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
		{
			if ((key[i] == '+' && !key[i + 1]) && i != 0)
			{
				if (exported[i + 1] != '=')
					return (false);
				return (true);
			}
			else
				return (false);
		}
	}
	return (true);
}

void	load_to_exp(char ***export, char *args, char *key_arg, t_data *data)
{
	char	*sub;
	char	**key;
	int		j;

	j = -1;
	while ((*export)[++j])
	{
		sub = ft_substr((*export)[j], 11, ft_strlen((*export)[j]));
		check_error_null(sub, "malloc");
		key = ft_split(sub, '=');
		check_error_null(key, "malloc");
		if (ft_strncmp(key_arg, key[0], ft_strlen(key[0])) == 0
			&& ft_strlen(key_arg) == ft_strlen(key[0]))
		{
			if (ft_strchr(args, '='))
				continue_loading(export, args, j, data);
			data->found = 1;
		}
		(1) && (free_array(key), free(sub), key = NULL);
	}
}

void	export_to_exp(char ***export, char **args, t_data *data)
{
	int		i;
	char	**key_arg;

	i = 0;
	while (args[++i])
	{
		data->found = 0;
		if (!check_exported(args[i], 0, data))
			continue ;
		key_arg = ft_split(args[i], '=');
		check_error_null(key_arg, "malloc");
		if (data->plus)
		{
			data->temp = key_arg[0];
			key_arg[0] = ft_substr(key_arg[0], 0, ft_strlen(key_arg[0]) - 1);
			free(data->temp);
		}
		load_to_exp(export, args[i], key_arg[0], data);
		if (!data->found)
			add_var(export, ft_strjoin("declare -x ", args[i]), 1, data->plus);
		free_double(key_arg);
	}
	data->found = 0;
}

void	export_to_env(char ***env, char **key_val, t_data *data, int *found)
{
	size_t	k;
	int		i;

	i = -1;
	while ((*env)[++i])
	{
		k = 0;
		while ((*env)[i][k] && (*env)[i][k] != '=')
			k++;
		if ((ft_strncmp((*env)[i], key_val[0], k) == 0)
			&& (k == ft_strlen(key_val[0])))
		{
			data->temp = (*env)[i];
			if (data->plus)
				(*env)[i] = ft_strjoin((*env)[i], key_val[1]);
			else
			{
				(*env)[i] = ft_strjoin(key_val[0], "=");
				check_error_null((*env)[i], "malloc");
				(1) && (free(data->temp), data->temp = (*env)[i]);
				(*env)[i] = ft_strjoin((*env)[i], key_val[1]);
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

	j = 0;
	while (data->cmd->arguments[++j])
	{
		if (!check_exported(data->cmd->arguments[j], 1, data))
			continue ;
		key_val = key_value(data->cmd->arguments[j]);
		check_error_null(key_val, "malloc");
		if (ft_strchr(key_val[0], '+'))
		{
			(1) && (data->temp = key_val[0], data->plus = 1);
			key_val[0] = ft_substr(key_val[0], 0, ft_strlen(key_val[0]) - 1);
			free(data->temp);
		}
		found = 0;
		export_to_env(env, key_val, data, &found);
		free_double(key_val);
		if (!found)
			add_var(env, data->cmd->arguments[j], 0, data->plus);
	}
	export_to_exp(&data->export, data->cmd->arguments, data);
	data->plus = 0;
}
