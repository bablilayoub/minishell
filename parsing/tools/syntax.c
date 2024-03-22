/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:23:16 by abablil           #+#    #+#             */
/*   Updated: 2024/03/22 05:52:04 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int	check_quotes(t_token *token)
{
	t_token	*tmp;
	int		count;

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

int	check_pipes(t_cmd *cmd)
{
	t_cmd	*tmp;

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

int	check_redirections(t_cmd *cmd)
{
	t_cmd			*tmp;
	t_redirection	*redirects;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->has_redirection)
		{
			redirects = tmp->redirects;
			while (tmp->redirects)
			{
				if (!tmp->redirects->file)
				{
					printf("%s\n", PREFIX_ERROR "Syntax error");
					return (0);
				}
				tmp->redirects = tmp->redirects->next;
			}
			tmp->redirects = redirects;
		}
		tmp = tmp->next;
	}
	return (1);
}

int	check_syntax(t_data *data)
{
	if (!check_pipes(data->cmd) || !check_redirections(data->cmd))
	{
		data->exit_status = 258;
		return (0);
	}
	return (1);
}
