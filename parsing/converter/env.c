/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 17:03:47 by abablil           #+#    #+#             */
/*   Updated: 2024/03/10 22:23:57 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void handle_env_var(t_data *data, t_cmd *tmp,
					char *env_var, char *exit_status)
{
	if (!tmp->args)
		return ;
	if (tmp->args->arg && ft_strncmp(tmp->args->arg, "$", 1) == 0 && tmp->args->env_var == 1 && ft_strlen(tmp->args->arg) > 1)
	{
		if (tmp->args->arg && ft_strncmp(tmp->args->arg, "$?", 2) == 0)
		{
			exit_status = ft_itoa(data->exit_status);
			tmp->args->arg = ft_strjoin(exit_status, tmp->args->arg + 2);
		}
		else
		{
			env_var = get_env(tmp->args->arg + 1, data);
			if (!env_var)
				tmp->args->arg = ft_strdup("");
			else
				tmp->args->arg = ft_strdup(env_var);
		}
	}
	tmp->args = tmp->args->next;
}

void get_env_vars(t_data *data)
{
	t_token *tmp;

	tmp = data->token;
	while (tmp)
	{
		if (tmp->value && (ft_strncmp(tmp->value, "$?", 2) == 0) && tmp->state != IN_QUOTE)
		{
			tmp->value = ft_itoa(data->exit_status);
			tmp->type = ft_strdup(WORD);
		}
		else if (tmp->value && ft_strncmp(tmp->type, ENV, ft_strlen(ENV)) == 0 && ft_strlen(tmp->value) > 1 && tmp->state != IN_QUOTE && ft_strncmp(tmp->value, "$?", 2) != 0)
		{
			tmp->value = get_env(tmp->value + 1, data);
			if (!tmp->value)
			{
				tmp->value = ft_strdup("");
				tmp->type = ft_strdup(WHITE_SPACE);
			}
			else
				tmp->type = ft_strdup(WORD);
		}
		else if (tmp->value && ft_strncmp(tmp->value, "$", 1) == 0)
			tmp->type = ft_strdup(WORD);
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
