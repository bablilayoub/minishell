/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:42:27 by abablil           #+#    #+#             */
/*   Updated: 2024/02/21 18:06:11 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int not_a_shell_command(t_token *token)
{
	if (!token)
		return (0);
	if (ft_strncmp(token->type, PIPE_LINE, 1) == 0)
		return (0);
	if (ft_strncmp(token->type, REDIR_IN, 1) == 0)
		return (0);
	if (ft_strncmp(token->type, REDIR_OUT, 1) == 0)
		return (0);
	if (ft_strncmp(token->type, HERE_DOC, 2) == 0)
		return (0);
	if (ft_strncmp(token->type, APPEND_OUT, 2) == 0)
		return (0);
	return (1);
}

void print_args(t_cmd *head)
{
	t_cmd *tmp = head;
	t_arg *arg;
	
	if (!tmp)
		return;
	while (tmp)
	{
		printf("--------------------------------------------\n");
		printf("| Command  : %s %*s |\n", tmp->cmd, 28 - (int)ft_strlen(tmp->cmd), " ");
		if (tmp->args)
		{
			arg = tmp->args;
			while (tmp->args)
			{
				printf("| Arguement: '%s' %*s |  Env Var : %d    |\n", tmp->args->arg, 8 - (int)ft_strlen(tmp->args->arg), " ", tmp->args->env_var);
				tmp->args = tmp->args->next;
			}
			tmp->args = arg;
		}
		if (tmp->redirect)
			printf("| Redirect : '%s' %*s |\n", tmp->redirect, 26 - (int)ft_strlen(tmp->redirect), " ");
		else
			printf("| Redirect : '%s' %*s |\n", "NULL", 26 - 4, " ");
		if (tmp->file)
			printf("| File     : '%s' %*s |\n", tmp->file, 26 - (int)ft_strlen(tmp->file), " ");
		else
			printf("| Output   : '%s' %*s |\n", "NULL", 26 - 4, " ");
		if (tmp->has_pipe)
			printf("| Has Pipe : %d %*s |\n", tmp->has_pipe, 27, " ");
		else
			printf("| Has Pipe : %d %*s |\n", tmp->has_pipe, 27, " ");
		printf("--------------------------------------------\n");
		tmp = tmp->next;
	}
}

void convert_tokens_to_commands(t_data *data)
{
	if (!data->token)
		return;
	t_token *tmp = data->token;
	t_cmd *head = NULL;
	int found_cmd = 0;

	while (tmp)
	{
		t_cmd *cmd = NULL;
		found_cmd = 0;
		while (tmp && ft_strncmp(tmp->type, PIPE_LINE, 1) != 0)
		{
			if (ft_strncmp(tmp->type, WORD, 4) == 0 && !found_cmd)
			{
				found_cmd = 1;
				cmd = new_cmd(tmp);
				head = add_cmd(head, cmd);
				if (tmp->next)
				{
					tmp = find_args(cmd, tmp->next);
					break;
				}
				else
				{
					cmd->args = add_arg(cmd->args, cmd->cmd, 1);
					break;
				}
			}
			tmp = tmp->next;
			if (tmp)
				tmp = skip_white_spaces(tmp);
		}
		if (tmp)
			tmp = tmp->next;
		if (tmp && (ft_strncmp(tmp->type, PIPE_LINE, 1) == 0))
			tmp = tmp->next;
	}
	data->cmd = head;
	// print_args(data->cmd);
}