/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 08:51:56 by abablil           #+#    #+#             */
/*   Updated: 2024/02/17 11:25:49 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void handle_quotes(t_token_params *params, int quote, char *quote_type)
{
	if (quote == 1)
	{
		if (params->in_quote)
			params->in_quote = 0;
		else
			params->in_quote = 1;
	}
	else
	{
		if (params->in_dquote)
			params->in_dquote = 0;
		else
			params->in_dquote = 1;
	}
	if (quote == 1 && params->in_quote)
		params->state = IN_QUOTE;
	else if (quote == 2 && params->in_dquote)
		params->state = IN_DQUOTE;
	else
		params->state = GENERAL;
	params->value = ft_strdup(quote_type);
	params->type = ft_strdup(quote_type);
	params->token = new_token(params->value, params->type, GENERAL, ft_strlen(params->value));
	params->head = add_token(params->head, params->token);
}

void handle_word(t_token_params *params, char *line)
{
	params->value = get_word(line, &params->i);
	params->type = ft_strdup(WORD);
	params->token = new_token(params->value, params->type, params->state, ft_strlen(params->value));
	params->head = add_token(params->head, params->token);
}

void handle_special_char(t_token_params *params, char *value, int len)
{
	char *temp;
	
	if (value[0] == '$' && value[1] == '\0')
	{
		params->i++;
		temp = get_word(params->line, &params->i);
		params->value = ft_strjoin(value, temp);
		free(temp);
		params->i--;
		len = ft_strlen(params->value);
	} else
		params->value = ft_strdup(value);
	params->type = ft_strdup(value);
	params->token = new_token(params->value, params->type, params->state, len);
	params->head = add_token(params->head, params->token);
	if (len > 1)
		params->i++;
}

void handle_cases(t_token_params *params, char *line)
{
	if (line[params->i] == '\'')
		handle_quotes(params, 1, QUOTE);
	else if (line[params->i] == '\"')
		handle_quotes(params, 2, DOUBLE_QUOTE);
	else if (line[params->i] == ' ' || line[params->i] == '\t')
		handle_special_char(params, WHITE_SPACE, 1);
	else if (line[params->i] == '\n')
		handle_special_char(params, NEW_LINE, 1);
	else if (line[params->i] == '$')
		handle_special_char(params, ENV, 1);
	else if (line[params->i] == '|')
		handle_special_char(params, PIPE_LINE, 1);
	else if (line[params->i] == '<' && line[params->i + 1] == '<')
		handle_special_char(params, HERE_DOC, 2);
	else if (line[params->i] == '>' && line[params->i + 1] == '>')
		handle_special_char(params, APPEND_OUT, 2);
	else if (line[params->i] == '<')
		handle_special_char(params, REDIR_IN, 1);
	else if (line[params->i] == '>')
		handle_special_char(params, REDIR_OUT, 1);
	else if (line[params->i] == '\\')
		handle_special_char(params, ESCAPE, 1);
	else
		handle_word(params, line);
}

void *tokenizer(char *line)
{
	t_token_params params;

	params.i = 0;
	params.state = GENERAL;
	params.head = NULL;
	params.in_quote = 0;
	params.in_dquote = 0;
	params.line = line;
	while (params.i < ft_strlen(line) && line[params.i])
	{
		handle_cases(&params, line);
		params.i++;
	}
	return (params.head);
}
