/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:23:16 by abablil           #+#    #+#             */
/*   Updated: 2024/03/07 22:04:44 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int check_quotes(t_token *token)
{
	t_token *tmp;
	int count;

	count = 0;
	tmp = token;
	while (tmp)
	{
		if ((ft_strncmp(tmp->type, QUOTE, 1) == 0
			|| ft_strncmp(tmp->type, DOUBLE_QUOTE, 1) == 0)
			&& tmp->state == GENERAL)
			count++;
		tmp = tmp->next;
	}
	if (count % 2 != 0)
	{
		printf(PREFIX_ERROR "Syntax error\n");
		return (0);
	}
	return (1);
}

int check_pipes(t_cmd *cmd)
{
	t_cmd *tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->has_pipe)
		{
			if (!tmp->next)
			{
				printf("%s\n", PREFIX_ERROR "Syntax error");
				return (0);
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

int check_redirections(t_cmd *cmd)
{
	t_cmd *tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->has_redir_in)
		{
			while (tmp->redirect_in)
			{
				if ((ft_strncmp(tmp->redirect_in->type, REDIR_IN, 1) != 0
					&& ft_strncmp(tmp->redirect_in->type, HERE_DOC, 2) != 0) || !tmp->redirect_in->file)
				{
					printf("%s\n", PREFIX_ERROR "Syntax error");
					return (0);
				}
				tmp->redirect_in = tmp->redirect_in->next;
			}
		}
		if (tmp->has_redir_out)
		{
			while (tmp->redirect_out)
			{
				if ((ft_strncmp(tmp->redirect_out->type, REDIR_OUT, 1) != 0
					&& ft_strncmp(tmp->redirect_out->type, APPEND_OUT, 2) != 0) || !tmp->redirect_out->file)
				{
					printf("%s\n", PREFIX_ERROR "Syntax error");
					return (0);
				}
				tmp->redirect_out = tmp->redirect_out->next;
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

int check_syntax(t_data *data)
{
	if (!check_quotes(data->token))
		return (0);
	if (!check_pipes(data->cmd))
		return (0);
	if (!check_redirections(data->cmd))
		return (0);
	return (1);
}
