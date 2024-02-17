/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:31:53 by abablil           #+#    #+#             */
/*   Updated: 2024/02/17 08:58:59 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void parser(char *line, t_data *data)
{
	(void)data;

	if (only_spaces(line))
		return;

	// Tokenize the line
	data->token = tokenizer(line);
	print_tokens(data->token);
}

void reader(t_data *data)
{
	char *line;

	while (1)
	{
		line = readline(PREFIX);
		if (!line)
			break;
		parser(line, data);
		if (ft_strlen(line) > 0)
			add_history(line);
		free(line);
	}
}
