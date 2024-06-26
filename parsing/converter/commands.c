/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:50:50 by abablil           #+#    #+#             */
/*   Updated: 2024/03/19 01:10:56 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

bool	is_built_in(char *value)
{
	if (!value)
		return (false);
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
	check_error_null(cmd, "malloc");
	cmd->cmd = token->value;
	cmd->path = NULL;
	cmd->has_pipe = 0;
	cmd->arguments = NULL;
	cmd->has_redirection = 0;
	cmd->redirects = NULL;
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
