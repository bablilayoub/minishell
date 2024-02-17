/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:42:27 by abablil           #+#    #+#             */
/*   Updated: 2024/02/17 14:48:45 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void convert_tokens_to_commands(t_token *token)
{
	t_token *tmp = token;
	t_token *head = token;
	t_token *cmd;
	t_token *cmd_head;
	t_token *cmd_tmp;
	t_token *cmd_tmp_head;
	t_token *cmd_tmp_prev;
	t_token *cmd_tmp_next;

	while (tmp)
	{
		if (ft_strncmp(tmp->type, PIPE_LINE, 1) == 0)
		{
			cmd = head;
			cmd_head = head;
			cmd_tmp = cmd;
			cmd_tmp_head = cmd;
			cmd_tmp_prev = cmd;
			cmd_tmp_next = cmd;
			while (cmd_tmp)
			{
				if (ft_strncmp(cmd_tmp->type, PIPE_LINE, 1) == 0)
				{
					cmd_tmp_prev->next = NULL;
					cmd_tmp_next = cmd_tmp->next;
					cmd_tmp->next = NULL;
					break;
				}
				cmd_tmp_prev = cmd_tmp;
				cmd_tmp = cmd_tmp->next;
			}
			head = cmd_tmp_next;
		}
		tmp = tmp->next;
	}
	while (cmd_head)
	{
		printf("cmd: %s\n", cmd_head->value);
		cmd_head = cmd_head->next;
	}
}