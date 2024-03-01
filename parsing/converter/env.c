/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 17:03:47 by abablil           #+#    #+#             */
/*   Updated: 2024/03/01 22:49:43 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	handle_env_var(t_data *data, t_cmd *tmp,
	char *env_var, char *exit_status)
{
	if (ft_strncmp(tmp->args->arg, "$", 1) == 0 && tmp->args->env_var == 1)
	{
		if (ft_strncmp(tmp->args->arg, "$?", 2) == 0)
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

void	get_env_vars(t_data *data)
{
	t_cmd	*tmp;
	t_arg	*arg;
	char	*env_var;
	char	*exit_status;

	tmp = data->cmd;
	while (tmp)
	{
		if (ft_strncmp(tmp->cmd, "export", 6) == 0)
		{
			tmp = tmp->next;
			continue ;
		}
		arg = tmp->args;
		while (tmp->args)
		{
			env_var = NULL;
			exit_status = NULL;
			handle_env_var(data, tmp, env_var, exit_status);
		}
		tmp->args = arg;
		tmp = tmp->next;
	}
}

char	*get_env(char *env, t_data *data)
{
	int		i;
	int		j;
	char	*env_var;
	char	*env_value;

	i = 0;
	while (data->env[i])
	{
		j = 0;
		while (data->env[i][j] && data->env[i][j] != '=')
			j++;
		env_var = ft_substr(data->env[i], 0, j);
		if (ft_strncmp(env, env_var, ft_strlen(env)) == 0)
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
