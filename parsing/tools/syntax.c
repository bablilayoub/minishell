/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:23:16 by abablil           #+#    #+#             */
/*   Updated: 2024/03/01 16:32:41 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int	check_quotes(t_token *token)
{
	t_token	*tmp;
	int		quotes_count;
	int		dquotes_count;

	(1 == 1) && (tmp = token, quotes_count = 0, dquotes_count = 0);
	while (tmp)
	{
		if (ft_strncmp(tmp->type, QUOTE, 1) == 0)
			quotes_count++;
		tmp = tmp->next;
	}
	tmp = token;
	while (tmp)
	{
		if (ft_strncmp(tmp->type, DOUBLE_QUOTE, 1) == 0)
			dquotes_count++;
		tmp = tmp->next;
	}
	if (quotes_count % 2 != 0 || dquotes_count % 2 != 0)
	{
		printf("%s\n", PREFIX_ERROR "Syntax error");
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

int	check_syntax(t_data *data)
{
	if (!check_quotes(data->token))
		return (0);
	if (!check_pipes(data->cmd))
		return (0);
	return (1);
}
