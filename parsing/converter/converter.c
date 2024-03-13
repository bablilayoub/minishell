/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:42:27 by abablil           #+#    #+#             */
/*   Updated: 2024/03/13 01:33:14 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int	not_a_shell_command(t_token *token)
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

t_token	*get_command_name(t_token *head)
{
	t_token	*tmp;

	tmp = head;
	if (!tmp)
		return (NULL);
	while (tmp && not_a_shell_command(tmp)
		&& ft_strncmp(tmp->type, WORD, 4) != 0)
		tmp = tmp->next;
	return (tmp);
}

t_token *filtrate_tokens(t_data *data, t_token *head)
{
	t_token *new_head = NULL;
	t_token *tmp = NULL;
	t_token *holder = NULL;

	if (!head)
		return (NULL);
	while (head)
	{
		if ((ft_strncmp(head->type, DOUBLE_QUOTE, 1) == 0 || ft_strncmp(head->type, QUOTE, 1) == 0 || ft_strlen(head->value) == 0) && head->state == GENERAL)
		{
			head = head->next;
			continue;
		}
		new_head = add_token(new_head, new_token(ft_strdup(head->value), ft_strdup(head->type), head->state, head->len));
		head = head->next;
	}
	tmp = new_head;
	while (tmp)
	{
		while (tmp && (ft_strncmp(tmp->type, WORD, 4) == 0))
		{
			if (tmp->next && ft_strncmp(tmp->next->type, WORD, 4) == 0)
			{
				holder = tmp->next;
				data->temp = tmp->value;
				tmp->value = ft_strjoin(tmp->value, tmp->next->value);
				free(data->temp);
				tmp->len = ft_strlen(tmp->value);
				free_tokens(tmp->next);
				tmp->next = tmp->next->next;
				free_token(holder);
			}
			else
				break;
		}
		while (tmp && (ft_strncmp(tmp->type, WHITE_SPACE, 1) != 0 || ft_strlen(tmp->value) == 0))
		{
			if (tmp->next && ft_strlen(tmp->next->value) == 0)
			{
				holder = tmp->next;
				data->temp = tmp->type;
				tmp->type = ft_strdup(tmp->next->type);
				free(data->temp);
				data->temp = tmp->value;
				tmp->value = ft_strjoin(tmp->value, tmp->next->value);
				free(data->temp);
				tmp->len = ft_strlen(tmp->value);
				free_tokens(tmp->next);
				tmp->next = tmp->next->next;
				free_token(holder);
			}
			else
				break;
		}
		if (tmp)
			tmp = tmp->next;
	}
	free_tokens(data->token);
	data->token = new_head;
	return (new_head);
}

void convert_tokens_to_commands(t_data *data)
{
	if (!data->token)
		return;
	t_token *tmp = data->token;
	t_cmd *head = NULL;
	int found_cmd = 0;

	tmp = filtrate_tokens(data, tmp);
	// print_tokens(tmp);
	tmp = skip_white_spaces(tmp);
	if (!tmp)
		return;
	if (ft_strncmp(tmp->type, WORD, 1) != 0 && ft_strncmp(tmp->type, APPEND_OUT, 2) != 0 && ft_strncmp(tmp->type, REDIR_OUT, 1) != 0 && ft_strncmp(tmp->type, HERE_DOC, 2) != 0 && ft_strncmp(tmp->type, REDIR_IN, 1) != 0 && ft_strncmp(tmp->type, ENV, 1) != 0)
	{
		printf("%s\n", PREFIX_ERROR "Syntax error");
		return;
	}
	while (tmp)
	{
		t_cmd *cmd = NULL;
		found_cmd = 0;
		while (tmp)
		{
			if (ft_strncmp(tmp->type, PIPE_LINE, 1) == 0 && tmp->state != IN_DQUOTE && tmp->state != IN_QUOTE)
				break;
			if ((ft_strncmp(tmp->type, WORD, 4) == 0 || ft_strncmp(tmp->type, ENV, 1) == 0) && !found_cmd)
			{
				found_cmd = 1;
				cmd = new_cmd(tmp);
				head = add_cmd(head, cmd);
				if (tmp->next)
					tmp = find_args(cmd, tmp->next);
				else
					cmd->args = add_arg(cmd->args, cmd->cmd, 1);
				break;
			}
			else if (((ft_strncmp(tmp->type, APPEND_OUT, 2) == 0 || ft_strncmp(tmp->type, REDIR_OUT, 1) == 0) || (ft_strncmp(tmp->type, HERE_DOC, 2) == 0 || ft_strncmp(tmp->type, REDIR_IN, 1) == 0)) && !found_cmd)
			{
				found_cmd = 1;
				cmd = new_cmd(tmp);
				while (tmp && ((ft_strncmp(tmp->type, APPEND_OUT, 2) == 0 || ft_strncmp(tmp->type, REDIR_OUT, 1) == 0) || (ft_strncmp(tmp->type, HERE_DOC, 2) == 0 || ft_strncmp(tmp->type, REDIR_IN, 1) == 0)))
				{
					if (ft_strncmp(tmp->type, APPEND_OUT, 2) == 0 || ft_strncmp(tmp->type, REDIR_OUT, 1) == 0)
						cmd->has_redir_out = 1;
					else if (ft_strncmp(tmp->type, HERE_DOC, 2) == 0 || ft_strncmp(tmp->type, REDIR_IN, 1) == 0)
						cmd->has_redir_in = 1;
					if (ft_strncmp(tmp->type, APPEND_OUT, 2) == 0)
						tmp = add_file(&cmd->redirect_out, tmp->next, APPEND_OUT);
					else if (ft_strncmp(tmp->type, REDIR_OUT, 1) == 0)
						tmp = add_file(&cmd->redirect_out, tmp->next, REDIR_OUT);
					else if (ft_strncmp(tmp->type, HERE_DOC, 2) == 0)
						tmp = add_file(&cmd->redirect_in, tmp->next, HERE_DOC);
					else if (ft_strncmp(tmp->type, REDIR_IN, 1) == 0)
						tmp = add_file(&cmd->redirect_in, tmp->next, REDIR_IN);
					if (tmp)
						tmp = tmp->next;
					if (tmp)
						tmp = skip_white_spaces(tmp);
				}
				if (tmp)
					cmd->cmd = tmp->value;
				else
					cmd->cmd = NULL;
				cmd->built_in = is_built_in(cmd->cmd);
				head = add_cmd(head, cmd);
				tmp = get_command_name(tmp);
				tmp = skip_white_spaces(tmp);
				if (tmp && tmp->next)
					tmp = find_args(cmd, tmp->next);
				else
					cmd->args = add_arg(cmd->args, cmd->cmd, 1);
				break;
			}
			if (tmp)
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
	print_args(data->cmd);
}
