/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:42:27 by abablil           #+#    #+#             */
/*   Updated: 2024/04/01 00:49:08 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	convert_word(t_token **tmp, t_cmd **head, t_cmd **cmd, int *found_cmd)
{
	*found_cmd = 1;
	if ((*tmp)->value && ft_strlen((*tmp)->value) > 0)
		*cmd = new_cmd(*tmp);
	else
	{
		free((*tmp)->value);
		(*tmp)->value = ft_strdup(" ");
		free((*tmp)->type);
		(*tmp)->type = ft_strdup(SPECIAL_CASE);
		*cmd = new_cmd(*tmp);
	}
	*head = add_cmd(*head, *cmd);
	if ((*tmp)->next)
		*tmp = find_args(*cmd, (*tmp)->next);
	else
		(*cmd)->args = add_arg((*cmd)->args, (*cmd)->cmd, 1);
}

void	finish_cmd(t_cmd **cmd, t_token **tmp, t_cmd **head)
{
	(*cmd)->built_in = is_built_in((*cmd)->cmd);
	*head = add_cmd(*head, *cmd);
	if (*tmp && ft_strncmp((*tmp)->type, PIPE_LINE, 1) == 0)
	{
		(*tmp) = (*tmp)->prev;
		(*cmd)->has_pipe = 1;
		return ;
	}
	*tmp = get_command_name(*tmp);
	*tmp = skip_white_spaces(*tmp);
	if (*tmp && (*tmp)->next)
		*tmp = find_args(*cmd, (*tmp)->next);
	else
		(*cmd)->args = add_arg((*cmd)->args, (*cmd)->cmd, 1);
}

void	convert_redirection(t_token **tmp, t_cmd **head,
	t_cmd **cmd, int *found_cmd)
{
	*found_cmd = 1;
	*cmd = new_cmd(*tmp);
	while (*tmp && ((ft_strncmp((*tmp)->type, APPEND_OUT, 2) == 0
				|| ft_strncmp((*tmp)->type, REDIR_OUT, 1) == 0)
			|| (ft_strncmp((*tmp)->type, HERE_DOC, 2) == 0
				|| ft_strncmp((*tmp)->type, REDIR_IN, 1) == 0)))
	{
		if (ft_strncmp((*tmp)->type, APPEND_OUT, 2) == 0
			|| ft_strncmp((*tmp)->type, REDIR_OUT, 1) == 0
			|| ft_strncmp((*tmp)->type, HERE_DOC, 2) == 0
			|| ft_strncmp((*tmp)->type, REDIR_IN, 1) == 0)
			*tmp = add_file(cmd, &(*cmd)->redirects,
					(*tmp)->next, (*tmp)->type);
		if (*tmp)
			*tmp = (*tmp)->next;
		if (*tmp)
			*tmp = skip_white_spaces(*tmp);
	}
	if (*tmp && ft_strncmp((*tmp)->type, WORD, 4) == 0)
		(*cmd)->cmd = (*tmp)->value;
	else
		(*cmd)->cmd = NULL;
	finish_cmd(cmd, tmp, head);
}

void print_tokens(t_token *token)
{
	t_token *tmp;
	int i;
	int space_left;

	tmp = token;
	printf("-------------------------------------------------------------------------------\n");
	printf("|    Value           |    Lenght        |    State         |    Type          |\n");
	printf("-------------------------------------------------------------------------------\n");
	while (tmp)
	{
		i = 0;
		space_left = 14 - ft_strlen(tmp->value);
		printf("|    '%s'", tmp->value);
		while (i < space_left)
		{
			printf(" ");
			i++;
		}
		i = 0;
		space_left = 14 - ft_strlen(ft_itoa(tmp->len));
		printf("|    %zu", tmp->len);
		while (i < space_left)
		{
			printf(" ");
			i++;
		}
		i = 0;
		switch (tmp->state)
		{
		case 0:
			printf("|    GENERAL");
			break;
		case 1:
			printf("|    IN_QUOTE");
			break;
		case 2:
			printf("|    IN_DQUOTE");
			break;
		}
		switch (tmp->state)
		{
		case 0:
			space_left = 14 - ft_strlen("GENERAL");
			break;
		case 1:
			space_left = 14 - ft_strlen("IN_QUOTE");
			break;
		case 2:
			space_left = 14 - ft_strlen("IN_DQUOTE");
			break;
		}
		while (i < space_left)
		{
			printf(" ");
			i++;
		}
		i = 0;
		char *new_type;
		if (ft_strncmp(tmp->type, QUOTE, 6) == 0)
			new_type = "QUOTE";
		else if (ft_strncmp(tmp->type, DOUBLE_QUOTE, 6) == 0)
			new_type = "DOUBLE_QUOTE";
		else if (ft_strncmp(tmp->type, WORD, 4) == 0)
			new_type = "WORD";
		else if (ft_strncmp(tmp->type, WHITE_SPACE, 1) == 0)
			new_type = "WHITE_SPACE";
		else if (ft_strncmp(tmp->type, NEW_LINE, 1) == 0)
			new_type = "NEW_LINE";
		else if (ft_strncmp(tmp->type, ESCAPE, 1) == 0)
			new_type = "ESCAPE";
		else if (ft_strncmp(tmp->type, ENV, 1) == 0)
			new_type = "ENV";
		else if (ft_strncmp(tmp->type, PIPE_LINE, 1) == 0)
			new_type = "PIPE_LINE";
		else if (ft_strncmp(tmp->type, REDIR_IN, 1) == 0)
			new_type = "REDIR_IN";
		else if (ft_strncmp(tmp->type, REDIR_OUT, 1) == 0)
			new_type = "REDIR_OUT";
		else if (ft_strncmp(tmp->type, HERE_DOC, 2) == 0)
			new_type = "HERE_DOC";
		else if (ft_strncmp(tmp->type, APPEND_OUT, 2) == 0)
			new_type = "APPEND_OUT";
		else if (ft_strncmp(tmp->type, TAB_SPACE, 1) == 0)
			new_type = "TAB_SPACE";
		else
			new_type = tmp->type;
		space_left = 14 - ft_strlen(new_type);
		printf("|    %s", new_type);
		while (i < space_left)
		{
			printf(" ");
			i++;
		}
		printf("|\n");
		tmp = tmp->next;
	}
	printf("-------------------------------------------------------------------------------\n");
}

void	start_converting(t_token **tmp, t_cmd **head, t_cmd **cmd)
{
	int	found_cmd;

	found_cmd = 0;
	if (*tmp)
	{
		if (ft_strncmp((*tmp)->type, PIPE_LINE, 1) == 0
			&& (*tmp)->state != IN_DQUOTE && (*tmp)->state != IN_QUOTE)
			return ;
		if ((ft_strncmp((*tmp)->type, WORD, 4) == 0
				|| ft_strncmp((*tmp)->type, ENV, 1) == 0) && !found_cmd)
			convert_word(tmp, head, cmd, &found_cmd);
		else if (cmd_starts_with_redir(*tmp) && !found_cmd)
			convert_redirection(tmp, head, cmd, &found_cmd);
		else if (*tmp && !found_cmd && ft_strncmp((*tmp)->type,
				SPECIAL_CASE, ft_strlen(SPECIAL_CASE)) == 0)
		{
			found_cmd = 1;
			(*tmp)->value = ft_strdup(" ");
			*cmd = new_cmd(*tmp);
			*head = add_cmd(*head, *cmd);
			while (*tmp && ft_strncmp((*tmp)->type, PIPE_LINE, 1) != 0)
				(*tmp) = (*tmp)->next;
		}
	}
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
		if (tmp->has_redirection)
		{
			t_redirection *redir = tmp->redirects;
			printf("|------------------------------------------|\n");
			printf("| Redirection %*s |\n", 34, " ");
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

int	convert_tokens_to_commands(t_data *data)
{
	t_token	*tmp;
	t_cmd	*head;
	t_cmd	*cmd;

	if (!data->token)
		return (0);
	tmp = data->token;
	head = NULL;
	cmd = NULL;
	print_tokens(tmp);
	tmp = clean_tokens(data, tmp);
	tmp = skip_white_spaces(tmp);
	if (!tmp || !check_syntax_error(tmp, data))
		return (0);
	while (tmp)
	{
		cmd = NULL;
		start_converting(&tmp, &head, &cmd);
		if (tmp)
			tmp = tmp->next;
		if (tmp && (ft_strncmp(tmp->type, PIPE_LINE, 1) == 0)
			&& tmp->state == GENERAL)
			tmp = tmp->next;
	}
	data->cmd = head;
	print_args(head);
	return (1);
}
