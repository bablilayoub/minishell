/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 15:45:48 by abablil           #+#    #+#             */
/*   Updated: 2024/03/11 22:48:52 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

t_arg *new_arg(char *value)
{
	t_arg *arg = (t_arg *)malloc(sizeof(t_arg));
	arg->arg = value;
	arg->next = NULL;
	arg->prev = NULL;
	return (arg);
}

t_arg *add_arg(t_arg *head, char *value, int found_quote)
{
	t_arg *arg = new_arg(value);
	if (found_quote)
		arg->env_var = 0;
	else
		arg->env_var = 1;
	t_arg *tmp = head;
	if (!head)
		return (arg);
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = arg;
	arg->prev = tmp;
	return (head);
}

bool between_dquotes(t_token *token)
{
	t_token *tmp = token;
	int count = 0;
	while (tmp && ft_strncmp(tmp->type, QUOTE, 1) != 0)
	{
		if (ft_strncmp(tmp->type, DOUBLE_QUOTE, 1) == 0)
			count++;
		tmp = tmp->prev;
	}
	return (count % 2 == 0);
}

t_token *find_args(t_cmd *cmd, t_token *token)
{
	if (!token || !cmd)
		return (NULL);
	t_token *tmp;
	t_arg *head;
	char *tmp_type;

	tmp = token;
	head = NULL;

	head = add_arg(head, cmd->cmd, 1);
	tmp = skip_white_spaces(tmp);
	while (tmp) 
	{
		if (ft_strncmp(tmp->type, PIPE_LINE, 1) == 0 && tmp->state != IN_DQUOTE && tmp->state != IN_QUOTE)
			break;
		if (ft_strncmp(tmp->type, WORD, 4) == 0)
			head = add_arg(head, tmp->value, 1);
		else if (ft_strncmp(tmp->type, ENV, 1) == 0)
			head = add_arg(head, tmp->value, (tmp->state == IN_QUOTE));
		else if ((ft_strncmp(tmp->type, REDIR_OUT, 1) == 0 || ft_strncmp(tmp->type, APPEND_OUT, 2) == 0) && (tmp->state != IN_QUOTE && tmp->state != IN_DQUOTE))
		{
			tmp_type = tmp->type;
			if (tmp->next)
				tmp = skip_white_spaces(tmp->next);
			if (tmp && ft_strncmp(tmp->type, WORD, 4) == 0)
			{
				cmd->has_redir_out = 1;
				if (ft_strncmp(tmp_type, APPEND_OUT, 2) == 0)
					tmp = add_file(&cmd->redirect_out, tmp, APPEND_OUT);
				else if (ft_strncmp(tmp_type, REDIR_OUT, 1) == 0)
					tmp = add_file(&cmd->redirect_out, tmp, REDIR_OUT);
			} 
			else 
			{
				cmd->has_redir_out = 1;
				tmp = add_file(&cmd->redirect_out, NULL, REDIR_OUT);
			}
		}
		else if ((ft_strncmp(tmp->type, REDIR_IN, 1) == 0) && (tmp->state != IN_QUOTE && tmp->state != IN_DQUOTE))
		{
			tmp_type = tmp->type;
			if (tmp->next)
				tmp = skip_white_spaces(tmp->next);
			if (tmp && ft_strncmp(tmp->type, WORD, 4) == 0)
			{
				cmd->has_redir_in = 1;
				if (ft_strncmp(tmp_type, HERE_DOC, 2) == 0)
					tmp = add_file(&cmd->redirect_in, tmp, HERE_DOC);
				else if (ft_strncmp(tmp_type, REDIR_IN, 1) == 0)
					tmp = add_file(&cmd->redirect_in, tmp, REDIR_IN);
			} 
			else 
			{
				cmd->has_redir_in = 1;
				tmp = add_file(&cmd->redirect_in, NULL, REDIR_IN);
			}
		}
		else if ((ft_strncmp(tmp->type, REDIR_IN, 1) == 0 || ft_strncmp(tmp->type, REDIR_OUT, 1) == 0 || ft_strncmp(tmp->type, APPEND_OUT, 2) == 0 || ft_strncmp(tmp->type, HERE_DOC, 2) == 0) && (tmp->state == IN_QUOTE || tmp->state == IN_DQUOTE))
			head = add_arg(head, tmp->value, 1);
		else if (ft_strncmp(tmp->type, WHITE_SPACE, 1) == 0 && (tmp->state != GENERAL))
			head = add_arg(head, tmp->value, 1);
		else if (ft_strncmp(tmp->type, WHITE_SPACE, 1) == 0 && (tmp->state == GENERAL))
		{
			tmp = skip_white_spaces(tmp);
			if (tmp && (ft_strncmp(cmd->cmd, "echo", 4) == 0) && not_a_shell_command(tmp))
				head = add_arg(head, " ", 1);
			continue;
		}
		else if (ft_strncmp(tmp->type, QUOTE, 1) == 0 && between_dquotes(tmp) && (tmp->state == IN_DQUOTE))
			head = add_arg(head, tmp->value, 1);
		else if (ft_strncmp(tmp->type, PIPE_LINE, 1) == 0 && (tmp->state == IN_QUOTE || tmp->state == IN_DQUOTE))
			head = add_arg(head, tmp->value, 1);
		else if ((ft_strncmp(tmp->type, NEW_LINE, 1) == 0 || ft_strncmp(tmp->type, TAB_SPACE, 1) == 0) && (tmp->state == IN_QUOTE || tmp->state == IN_DQUOTE))
			head = add_arg(head, tmp->value, 1);
		else if (ft_strncmp(tmp->type, ESCAPE, 1) == 0 && (tmp->state == IN_QUOTE))
			head = add_arg(head, tmp->value, 1);
		else if (ft_strncmp(tmp->type, ESCAPE, 1) == 0 && (tmp->state == IN_DQUOTE) && !tmp->next)
		{
			printf(PREFIX_ERROR "Syntax error\n");
			return (NULL);
		}
		if (tmp && tmp->next)
			tmp = tmp->next;
		else
			break;
	}
	if (tmp && ft_strncmp(tmp->type, PIPE_LINE, 1) == 0 && tmp->state != IN_DQUOTE && tmp->state != IN_QUOTE)
		cmd->has_pipe = 1;
	cmd->args = head;
	return (tmp);
}
