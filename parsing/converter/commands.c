/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:50:50 by abablil           #+#    #+#             */
/*   Updated: 2024/03/07 01:27:42 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

bool	is_built_in(char *value)
{
	if (ft_strncmp(value, "echo", 5) == 0)
		return (true);
	if (ft_strncmp(value, "cd", 3) == 0)
		return (true);
	if (ft_strncmp(value, "pwd", 4) == 0)
		return (true);
	if (ft_strncmp(value, "export", 7) == 0)
		return (true);
	if (ft_strncmp(value, "unset", 6) == 0)
		return (true);
	if (ft_strncmp(value, "env", 4) == 0)
		return (true);
	if (ft_strncmp(value, "exit", 5) == 0)
		return (true);
	return (false);
}

t_cmd	*new_cmd(t_token *token)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	cmd->cmd = token->value;
	cmd->path = NULL;
	cmd->has_pipe = 0;
	cmd->arguments = NULL;
	cmd->has_redir_in = 0;
	cmd->has_redir_out = 0;
	cmd->redirect_in = NULL;
	cmd->redirect_out = NULL;
	cmd->args = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	cmd->built_in = is_built_in(token->value);
	return (cmd);
}

t_cmd	*add_cmd(t_cmd *head, t_cmd *cmd)
{
	t_cmd	*tmp;

	tmp = head;
	if (!head)
		return (cmd);
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = cmd;
	cmd->prev = tmp;
	return (head);
}

t_redirection	*new_redirect(char *type, char *file)
{
	t_redirection	*redirect;

	redirect = (t_redirection *)malloc(sizeof(t_redirection));
	redirect->type = type;
	redirect->file = file;
	redirect->next = NULL;
	return (redirect);
}

t_redirection	*add_redirect(t_redirection *head, t_redirection *redirect)
{
	t_redirection	*tmp;

	tmp = head;
	if (!head)
		return (redirect);
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = redirect;
	redirect->prev = tmp;
	return (head);
}

t_token	*add_file(t_redirection **head, t_token *token, char *type)
{
	if (!token)
		return (NULL);

	while (token && not_a_shell_command(token) && ft_strncmp(token->type, WORD, 4) != 0)
		token = token->next;
	if (!token)
		return (NULL);
	if (ft_strncmp(token->type, WORD, 4) == 0)
	{
		*head = add_redirect(*head, new_redirect(type, token->value));
		token = token->next;
	}
	return (token);
}
