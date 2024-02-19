/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 15:45:48 by abablil           #+#    #+#             */
/*   Updated: 2024/02/19 20:49:40 by abablil          ###   ########.fr       */
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

t_token *find_args(t_cmd *cmd, t_token *token)
{
	if (!token || !cmd)
		return (NULL);
	t_token *tmp = token;
	t_arg *head = NULL;

	head = add_arg(head, cmd->cmd, 1);
	while (tmp && ft_strncmp(tmp->type, PIPE_LINE, 1) != 0)
	{
		if (ft_strncmp(tmp->type, WORD, 4) == 0)
			head = add_arg(head, tmp->value, 1);
		else if (ft_strncmp(tmp->type, ENV, 1) == 0)
			head = add_arg(head, tmp->value, (tmp->state == IN_QUOTE));
		else if (ft_strncmp(tmp->type, REDIR_IN, 1) == 0
			|| ft_strncmp(tmp->type, REDIR_OUT, 1) == 0
			|| ft_strncmp(tmp->type, APPEND_OUT, 2) == 0
			|| ft_strncmp(tmp->type, HERE_DOC, 2) == 0)
		{
			cmd->redirect = tmp->type;
			if (tmp->next)
				tmp = skip_white_spaces(tmp->next);
			if (tmp && ft_strncmp(tmp->type, WORD, 4) == 0)
				cmd->output_file = tmp->value;
			else if (tmp && 
				(ft_strncmp(tmp->type, REDIR_IN, 1) == 0 
				|| ft_strncmp(tmp->type, REDIR_OUT, 1) == 0))
			{
				printf("%s\n", PREFIX_ERROR "Syntax error");
				return (NULL);
			} 
			else
				cmd->output_file = NULL;
		}
		else if (ft_strncmp(tmp->type, WHITE_SPACE, 1) == 0 && (tmp->state == IN_QUOTE || tmp->state == IN_DQUOTE))
			head = add_arg(head, tmp->value, 1);
		tmp = tmp->next;
	}
	if (tmp && ft_strncmp(tmp->type, PIPE_LINE, 1) == 0)
		cmd->has_pipe = 1;
	cmd->args = head;
	return (tmp);
}