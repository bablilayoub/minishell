/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:42:27 by abablil           #+#    #+#             */
/*   Updated: 2024/03/12 02:55:48 by abablil          ###   ########.fr       */
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
				if (!tmp->args->arg)
					break;
				if (ft_strncmp(tmp->args->arg, NEW_LINE, 1) != 0 && ft_strncmp(tmp->args->arg, TAB_SPACE, 1) != 0)
					printf("| Arguement: '%s' %*s |  Env Var : %d    |\n", tmp->args->arg, 8 - (int)ft_strlen(tmp->args->arg), " ", tmp->args->env_var);
				else if (ft_strncmp(tmp->args->arg, NEW_LINE, 1) == 0)
					printf("| Arguement: '%s' %*s |  Env Var : %d    |\n", "\\n", 8 - 2, " ", tmp->args->env_var);
				else
					printf("| Arguement: '%s' %*s |  Env Var : %d    |\n", "\\t", 8 - 2, " ", tmp->args->env_var);
				tmp->args = tmp->args->next;
			}
			tmp->args = arg;
		}
		if (tmp->has_redir_in)
		{
			t_redirection *redir = tmp->redirect_in;
			printf("|------------------------------------------|\n");
			printf("| Redirection In %*s |\n", 25, " ");
			while (redir)
			{
				printf("| Type     : '%s' %*s |\n", redir->type, 26 - (int)ft_strlen(redir->type), " ");
				printf("| File     : '%s' %*s |\n", redir->file, 26 - (int)ft_strlen(redir->file), " ");
				if (redir->next)
					printf("|%*s|\n", 42, " ");
				redir = redir->next;
			}
			printf("|------------------------------------------|\n");
		}
		else
			printf("| No Redirection In %*s |\n", 22, " ");
		if (tmp->has_redir_out)
		{
			t_redirection *redir = tmp->redirect_out;
			printf("|------------------------------------------|\n");
			printf("| Redirection Out %*s |\n", 24, " ");
			while (redir)
			{
				printf("| Type     : '%s' %*s |\n", redir->type, 26 - (int)ft_strlen(redir->type), " ");
				printf("| File     : '%s' %*s |\n", redir->file, 26 - (int)ft_strlen(redir->file), " ");
				if (redir->next)
					printf("|%*s|\n", 42, " ");
				redir = redir->next;
			}
			printf("|------------------------------------------|\n");
		}
		else
			printf("| No Redirection Out %*s |\n", 21, " ");
		if (tmp->has_pipe)
			printf("| Has Pipe : %d %*s |\n", tmp->has_pipe, 27, " ");
		else
			printf("| Has Pipe : %d %*s |\n", tmp->has_pipe, 27, " ");
		if (tmp->built_in)
			printf("| Built In : %d %*s |\n", tmp->built_in, 27, " ");
		else
			printf("| Built In : %d %*s |\n", tmp->built_in, 27, " ");
		printf("--------------------------------------------\n");
		tmp = tmp->next;
	}
}

t_token *get_command_name(t_token *head)
{
	t_token *tmp = head;
	if (!tmp)
		return (NULL);
	while (tmp && not_a_shell_command(tmp) && ft_strncmp(tmp->type, WORD, 4) != 0)
		tmp = tmp->next;
	return (tmp);
}

t_token *filtrate_tokens(t_data *data, t_token *head)
{
	t_token *new_head = NULL;
	t_token *tmp = NULL;

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
				tmp->value = ft_strjoin(tmp->value, tmp->next->value);
				tmp->len = ft_strlen(tmp->value);
				tmp->next = tmp->next->next;
			}
			else
				break;
		}
		while (tmp && (ft_strncmp(tmp->type, WHITE_SPACE, 1) != 0 || ft_strlen(tmp->value) == 0))
		{
			if (tmp->next && ft_strlen(tmp->next->value) == 0)
			{
				tmp->type = ft_strdup(tmp->next->type);
				tmp->value = ft_strjoin(tmp->value, tmp->next->value);
				tmp->len = ft_strlen(tmp->value);
				tmp->next = tmp->next->next;
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
	print_tokens(tmp);
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
	// print_args(data->cmd);
}