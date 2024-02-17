/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 15:45:48 by abablil           #+#    #+#             */
/*   Updated: 2024/02/17 19:31:00 by abablil          ###   ########.fr       */
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

void find_args(t_cmd *cmd, t_token *token)
{
	if (!token || !cmd)
		return;
	t_token *tmp = token;
	t_arg *head = NULL;

	head = add_arg(head, cmd->cmd, 0);
	while (not_a_shell_command(tmp))
	{
		if (ft_strncmp(tmp->type, WORD, 4) == 0)
			head = add_arg(head, tmp->value, 1);
		else if (ft_strncmp(tmp->type, ENV, 1) == 0)
			head = add_arg(head, tmp->value, (tmp->state == IN_QUOTE));
		tmp = tmp->next;
	}
	cmd->args = head;
}