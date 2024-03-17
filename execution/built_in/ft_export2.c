/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 03:53:30 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/16 03:06:07 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

bool	write_the_error(char **key_val, int flag, t_data *data)
{
	if (flag == 1)
		printf(PREFIX_ERROR "export: not an identifier\n");
	data->exit_status = 1;
	free_array(key_val);
	return (false);
}

int	check_exported(char *exported, int flag, t_data *data)
{
	int		i;
	char	**key_val;
	char	*key;

	(1) && (i = 0, key_val = key_value(exported));
	check_error_null(key_val, "malloc");
	key = key_val[0];
	if (ft_isdigit(key[i]) || (key[i] == '_' && !key[i + 1])
		|| !ft_is_alphanumeric(key) || !key[i])
	{
		if ((key[i] == '_' && !key[i + 1]))
		{
			free_array(key_val);
			return (false);
		}
		else
			return (write_the_error(key_val, flag, data));
	}
	if (!ft_strchr(exported, '=') && flag == 1)
	{
		free_array(key_val);
		return (false);
	}
	free_double(key_val);
	return (true);
}

char	**load_key_value(char *key, char *value)
{
	char	**key_value;

	key_value = malloc(sizeof(char *) * 3);
	if (!key_value)
	{
		free(key);
		free(value);
		exit(EXIT_FAILURE);
	}
	key_value[0] = key;
	key_value[1] = value;
	key_value[2] = NULL;
	return (key_value);
}

char	**key_value(char *exported)
{
	char	*value;
	char	*key;
	char	**key_value;
	int		i;
	int		k;

	i = 0;
	while (exported[i] && exported[i] != '=')
		i++;
	key = ft_substr(exported, 0, i);
	check_error_null(key, "malloc");
	k = i;
	while (exported[k])
		k++;
	value = ft_substr(exported, i + 1, k);
	check_error_null(value, "malloc");
	key_value = load_key_value(key, value);
	return (key_value);
}

void	add_var(char ***env, char *exported, int flag)
{
	int		i;
	char	**new_env;
	char	**temp;

	i = 0;
	new_env = malloc(sizeof(char *) * (ft_strdoublelen(*env) + 2));
	if (!new_env)
		exit(EXIT_FAILURE);
	while ((*env)[i])
	{
		new_env[i] = (*env)[i];
		i++;
	}
	if (!flag)
	{
		new_env[i] = ft_strdup(exported);
		check_error_null(new_env[i], "malloc");
	}
	else
		new_env[i] = exported;
	new_env[i + 1] = NULL;
	temp = *env;
	*env = new_env;
	free(temp);
}
