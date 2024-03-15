/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:41:45 by abablil           #+#    #+#             */
/*   Updated: 2024/03/15 04:46:59 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../parser.h"

void	free_redirections(t_redirection *redirections)
{
	t_redirection	*tmp;

	if (!redirections)
		return ;
	while (redirections)
	{
		tmp = redirections->next;
		free(redirections);
		redirections = tmp;
	}
}

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;

	if (!cmd)
		return ;
	while (cmd)
	{
		if (cmd->arguments)
			free_array(cmd->arguments);
		if (cmd->args)
			free_args(cmd->args);
		if (cmd->redirect_in)
			free_redirections(cmd->redirect_in);
		if (cmd->redirect_out)
			free_redirections(cmd->redirect_out);
		if (cmd->path)
			free(cmd->path);
		tmp = cmd->next;
		free(cmd);
		cmd = tmp;
	}
}

void	free_token(t_token *token)
{
	if (!token)
		return ;
	if (token->type)
		free(token->type);
	if (token->value)
		free(token->value);
	free(token);
	token = NULL;
}

void	free_tokens(t_token *token)
{
	t_token	*tmp;

	if (!token)
		return ;
	while (token)
	{
		tmp = token->next;
		free_token(token);
		token = tmp;
	}
	token = NULL;
}

void	free_data(t_data *data, int free_all)
{
	if (data->cmd)
		free_cmd(data->cmd);
	if (data->token)
		free_tokens(data->token);
	if (free_all)
	{
		free_array(data->env);
		free_array(data->export);
		free(data->shell_path);
		free(data->prefix);
		data->env = NULL;
		data->export = NULL;
		data->shell_path = NULL;
		data->prefix = NULL;
	}
	data->cmd = NULL;
	data->token = NULL;
}
