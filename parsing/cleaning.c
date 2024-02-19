/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:41:45 by abablil           #+#    #+#             */
/*   Updated: 2024/02/19 22:08:58 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_free_array(char **array)
{
	int i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	if (array)
		free(array);
}

void free_args(t_arg *args)
{
	t_arg *tmp;

	if (!args)
		return;
	while (args)
	{
		tmp = args->next;
		free(args);
		args = tmp;
	}
}

void free_cmd(t_cmd *cmd)
{
	t_cmd *tmp;
	
	while (cmd)
	{
		if (cmd->arguments)
			ft_free_array(cmd->arguments);
		if (cmd->args)
			free_args(cmd->args);
		tmp = cmd->next;
		free(cmd);
		cmd = tmp;
	}
}

void free_tokens(t_token *token)
{
	t_token *tmp;

	while (token)
	{
		tmp = token->next;
		free(token->value);
		free(token->type);
		free(token);
		token = tmp;
	}
}

void free_data(t_data *data)
{
	if (data->cmd)
		free_cmd(data->cmd);
	if (data->token)
		free_tokens(data->token);
	data->cmd = NULL;
	data->token = NULL;
}