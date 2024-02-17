/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:42:27 by abablil           #+#    #+#             */
/*   Updated: 2024/02/17 23:26:49 by abablil          ###   ########.fr       */
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
		if (ft_strncmp(tmp->type, WORD, 4) == 0 && !found_cmd)
		{
			cmd = new_cmd(tmp);
			head = add_cmd(head, cmd);
			found_cmd = 1;
			find_args(cmd, tmp->next);
		}
		else if (ft_strncmp(tmp->type, PIPE_LINE, 1) == 0 
				|| ft_strncmp(tmp->type, HERE_DOC, 2) == 0 
				|| ft_strncmp(tmp->type, APPEND_OUT, 2) == 0
				|| ft_strncmp(tmp->type, REDIR_IN, 1) == 0
				|| ft_strncmp(tmp->type, REDIR_OUT, 1) == 0)
		{
			cmd = new_cmd(tmp);
			found_cmd = 0;
			if (ft_strncmp(tmp->type, PIPE_LINE, 1) == 0)
				cmd->redirect = ft_strdup(PIPE_LINE);
			else if (ft_strncmp(tmp->type, REDIR_IN, 1) == 0)
				cmd->redirect = ft_strdup(REDIR_IN);
			else if (ft_strncmp(tmp->type, REDIR_OUT, 1) == 0)
				cmd->redirect = ft_strdup(REDIR_OUT);
			else if (ft_strncmp(tmp->type, HERE_DOC, 2) == 0)
				cmd->redirect = ft_strdup(HERE_DOC);
			else if (ft_strncmp(tmp->type, APPEND_OUT, 2) == 0)
				cmd->redirect = ft_strdup(APPEND_OUT);
			if (tmp->next && tmp->next->next && ft_strncmp(tmp->next->next->type, WORD, 4) == 0)
			{
				cmd->output_file = ft_strdup(tmp->next->next->value);
				tmp = tmp->next->next;
			}
			cmd->args = NULL;
			head = add_cmd(head, cmd);
		}
		tmp = tmp->next;
	}
	data->cmd = head;
	while (head)
	{
		printf("cmd: '%s'\n", head->cmd);
		if (head->args)
		{
			t_arg *arg = head->args;
			printf("args: ");
			while (arg)
			{
				printf(" '%s'", arg->arg);
				arg = arg->next;
			}
			printf("\n");
		}
		if (head->redirect)
			printf("redirect: '%s'\n", head->redirect);
		if (head->output_file)
			printf("output_file: '%s'\n", head->output_file);
		printf("\n");
		head = head->next;
	}
}