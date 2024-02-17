/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:42:27 by abablil           #+#    #+#             */
/*   Updated: 2024/02/17 17:45:01 by abablil          ###   ########.fr       */
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

int between_quotes(t_token *token)
{
	t_token *tmp = token;
	int found_quote = 0;
	while (not_a_shell_command(tmp))
	{
		if (ft_strncmp(tmp->type, QUOTE, 1) == 0)
			found_quote = 1;
		tmp = tmp->next;
	}
	return (found_quote);
}

void convert_tokens_to_commands(t_data *data)
{
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
}