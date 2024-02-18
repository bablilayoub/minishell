/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:42:27 by abablil           #+#    #+#             */
/*   Updated: 2024/02/18 13:20:01 by abablil          ###   ########.fr       */
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