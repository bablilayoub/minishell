/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:23:16 by abablil           #+#    #+#             */
/*   Updated: 2024/03/07 22:42:19 by abablil          ###   ########.fr       */
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
	t_redirection *redir_out;
	t_redirection *redir_in;
	tmp = cmd;
	while (tmp)
	{
		if (tmp->has_redir_in)
		{
			redir_in = tmp->redirect_in;
			while (tmp->redirect_in)
			{
				if (!tmp->redirect_in->file)
				{
					printf("%s\n", PREFIX_ERROR "Syntax error");
					return (0);
				}
				tmp->redirect_in = tmp->redirect_in->next;
			}
			tmp->redirect_in = redir_in;
		}
		if (tmp->has_redir_out)
		{
			redir_out = tmp->redirect_out;
			while (tmp->redirect_out)
			{
				if (!tmp->redirect_out->file)
				{
					printf("%s\n", PREFIX_ERROR "Syntax error");
					return (0);
				}
				tmp->redirect_out = tmp->redirect_out->next;
			}
			tmp->redirect_out = redir_out;
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
