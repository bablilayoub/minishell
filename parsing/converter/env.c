/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 17:03:47 by abablil           #+#    #+#             */
/*   Updated: 2024/03/11 22:12:58 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void get_env_vars(t_data *data)
{
	t_token *tmp;

	tmp = data->token;
	while (tmp)
	{
		if (tmp->value && (ft_strncmp(tmp->value, "$?", 2) == 0) && tmp->state != IN_QUOTE)
		{
			data->temp = tmp->value;
			tmp->value = ft_itoa(data->exit_status);
			free(data->temp);
			data->temp = tmp->type;
			tmp->type = ft_strdup(WORD);
			free(data->temp);
		}
		else if (tmp->value && ft_strncmp(tmp->type, ENV, ft_strlen(ENV)) == 0 && ft_strlen(tmp->value) > 1 && tmp->state != IN_QUOTE && ft_strncmp(tmp->value, "$?", 2) != 0)
		{
			data->temp = tmp->value;
			tmp->value = get_env(tmp->value + 1, data);
			free(data->temp);
			if (!tmp->value)
			{
				tmp->value = ft_strdup("");
				data->temp = tmp->type;
				tmp->type = ft_strdup(WHITE_SPACE);
				free(data->temp);
			}
			else
			{
				data->temp = tmp->type;
				tmp->type = ft_strdup(WORD);
				free(data->temp);
			}
		}
		else if (tmp->value && ft_strncmp(tmp->value, "$", 1) == 0)
		{
			data->temp = tmp->value;
			tmp->type = ft_strdup(WORD);
			free(data->temp);
		}
		tmp = tmp->next;
	}
}

char *get_env(char *env, t_data *data)
{
	int i;
	int j;
	char *env_var;
	char *env_value;

	i = 0;
	while (data->env[i])
	{
		j = 0;
		while (data->env[i][j] && data->env[i][j] != '=')
			j++;
		env_var = ft_substr(data->env[i], 0, j);
		if (ft_strncmp(env, env_var, ft_strlen(env_var)) == 0 && ft_strlen(env) == ft_strlen(env_var))
		{
			env_value = ft_strdup(data->env[i] + j + 1);
			free(env_var);
			return (env_value);
		}
		free(env_var);
		i++;
	}
	return (NULL);
}
