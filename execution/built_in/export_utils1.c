/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 03:53:30 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/27 22:25:40 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

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

char	*remove_if_found(char *str, int flag)
{
	int		k;
	char	*copy;
	char	**key_val;
	char	*temp;

	k = 0;
	while (str[k] && str[k] != '=')
		k++;
	key_val = key_value(str);
	check_error_null(key_val, "malloc");
	copy = ft_substr(key_val[0], 0, ft_strlen(key_val[0]) - 1);
	check_error_null(copy, "malloc");
	temp = copy;
	copy = ft_strjoin(copy, "=");
	free(temp);
	check_error_null(copy, "malloc");
	temp = copy;
	copy = ft_strjoin(copy, key_val[1]);
	free(temp);
	check_error_null(copy, "malloc");
	free_array(key_val);
	if (flag)
		free(str);
	return (copy);
}

void	add_var(char ***env, char *exported, int flag, int plus)
{
	int		i;
	char	**new_env;
	char	**temp;

	i = 0;
	new_env = malloc(sizeof(char *) * (ft_strdoublelen(*env) + 2));
	check_error_null(new_env, "malloc");
	while ((*env)[i])
	{
		new_env[i] = (*env)[i];
		i++;
	}
	if (plus)
		new_env[i] = remove_if_found(exported, flag);
	if (!flag && !plus)
		new_env[i] = ft_strdup(exported);
	else if (flag && !plus)
		new_env[i] = exported;
	new_env[i + 1] = NULL;
	temp = *env;
	*env = new_env;
	free(temp);
}
