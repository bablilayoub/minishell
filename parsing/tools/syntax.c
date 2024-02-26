/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:23:16 by abablil           #+#    #+#             */
/*   Updated: 2024/02/26 20:27:47 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void check_quotes(t_token *token)
{
	t_token *tmp = token;
	int quotes_count = 0;
	int dquotes_count = 0;
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
		printf("%s\n", PREFIX_ERROR "Syntax error");
}

void check_pipes(t_cmd *cmd)
{
	t_cmd *tmp = cmd;
	
	while (tmp)
	{
		if (tmp->has_pipe)
		{
			if (!tmp->next)
			{
				printf("%s\n", PREFIX_ERROR "Syntax error");
				return;
			}
		}
		tmp = tmp->next;
	}
}

void check_syntax(t_data *data)
{
	check_quotes(data->token);
	check_pipes(data->cmd);
}