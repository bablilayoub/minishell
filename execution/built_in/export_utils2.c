/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 13:55:50 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/27 22:25:46 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

bool	write_the_error(char **key_val, int flag, t_data *data)
{
	if (flag == 1)
		printf(PREFIX_ERROR "export: not a valid identifier\n");
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
	if (!key[i] || ft_isdigit(key[i]) || (key[i] == '_' && !key[i + 1])
		|| !ft_is_alphanumeric(key, exported))
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

void	continue_loading(char ***export, char *args, int j, t_data *data)
{
	char	*temp;
	char	**key_val;

	key_val = key_value(args);
	temp = (*export)[j];
	if (data->plus)
	{
		if (ft_strchr((*export)[j], '='))
			(*export)[j] = ft_strjoin((*export)[j], key_val[1]);
		else
		{
			(*export)[j] = ft_strjoin((*export)[j], "=");
			free(temp);
			temp = (*export)[j];
			(*export)[j] = ft_strjoin((*export)[j], key_val[1]);
		}
	}
	else
	{
		(*export)[j] = ft_strjoin("declare -x ", args);
		check_error_null((*export)[j], "malloc");
	}
	free(temp);
	free_double(key_val);
}
