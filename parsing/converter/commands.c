/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:50:50 by abablil           #+#    #+#             */
/*   Updated: 2024/02/17 16:51:05 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

t_cmd *new_cmd(t_token *token)
{
	t_cmd *cmd = (t_cmd *)malloc(sizeof(t_cmd));
	cmd->cmd = token->value;
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}

t_cmd *add_cmd(t_cmd *head, t_cmd *cmd)
{
	t_cmd *tmp = head;
	if (!head)
		return (cmd);
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = cmd;
	cmd->prev = tmp;
	return (head);
}