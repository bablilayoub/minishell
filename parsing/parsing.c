/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:31:53 by abablil           #+#    #+#             */
/*   Updated: 2024/02/17 18:03:45 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void validate_syntax(t_token *token)
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
		printf("%s\n", PREFIX "Syntax error");
}

void check_syntax(t_token *token)
{
	validate_syntax(token);
}

void parser(char *line, t_data *data)
{
	if (only_spaces(line))
		return;

	data->token = tokenizer(line);
	//print_tokens(data->token);
	check_syntax(data->token);
	convert_tokens_to_commands(data);
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
