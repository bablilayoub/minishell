/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 19:24:33 by alaalalm          #+#    #+#             */
/*   Updated: 2024/04/26 14:50:26 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	check_for_access(char **env, t_data *data, int *flag, t_cmd *head)
{
	int	i;

	i = 0;
	while (env[i])
	{
		data->temp = env[i];
		env[i] = ft_strjoin(env[i], "/");
		free(data->temp);
		data->temp = env[i];
		env[i] = ft_strjoin(env[i], head->cmd);
		free(data->temp);
		if (access(env[i], F_OK | X_OK) == 0)
		{
			head->path = ft_strdup(env[i]);
			*flag = 1;
			break ;
		}
		*flag = 0;
		i++;
	}
}

static void	error_print(int *found_error, t_cmd *head, t_data *data)
{
	printf(PREFIX_ERROR"%s: command not found\n", head->cmd);
	data->exit_status = 127;
	*found_error = 1;
}

void	init_path(t_data *data, t_cmd *head, int *flag, int *found_error)
{
	char	**env;

	while (head)
	{
		if (!head->cmd || head->built_in || access(head->cmd, F_OK | X_OK) == 0)
		{
			if (access(head->cmd, F_OK | X_OK) == 0)
				head->path = ft_strdup(head->cmd);
			head = head->next;
			continue ;
		}
		env = ft_split(ft_getenv("PATH", data->env), ':');
		if (!env)
		{
			failure(head->cmd, data);
			(1) && (head = head->next, free_array(env), env = NULL);
			continue ;
		}
		if (env)
			check_for_access(env, data, flag, head);
		if (!(*flag))
			error_print(found_error, head, data);
		(1) && (head = head->next, free_array(env), env = NULL);
	}
}

bool	initialize_path(t_cmd *head, t_data *data)
{
	int		flag;
	int		found_error;

	(1) && (flag = 0, found_error = 0);
	init_path(data, head, &flag, &found_error);
	if (found_error)
	{
		data->exit_status = 127;
		return (false);
	}
	return (true);
}

void	initialize_arguments(t_cmd *cmd_list)
{
	int		i;
	t_cmd	*temp;
	t_arg	*arg_temp;

	temp = cmd_list;
	while (temp)
	{
		temp->arguments = malloc(sizeof(char *)
				* (args_lenght(temp->args) + 1));
		check_error_null(temp->arguments, "malloc");
		i = 0;
		arg_temp = temp->args;
		while (temp->args)
		{
			temp->arguments[i++] = ft_strdup(temp->args->arg);
			check_error_null(temp->arguments[i - 1], "malloc");
			temp->args = temp->args->next;
		}
		temp->args = arg_temp;
		temp->arguments[i] = NULL;
		temp = temp->next;
	}
}
