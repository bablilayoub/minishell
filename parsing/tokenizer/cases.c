/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cases.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 21:18:29 by abablil           #+#    #+#             */
/*   Updated: 2024/03/01 16:06:45 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	handle_special_char(t_token_params *params, char *value, int len)
{
	char	*temp;

	if (value[0] == '$' && value[1] == '\0')
	{
		params->i++;
		temp = get_word(params->line, &params->i);
		params->value = ft_strjoin(value, temp);
		free(temp);
		params->i--;
		len = ft_strlen(params->value);
	}
	else
		params->value = ft_strdup(value);
	params->type = ft_strdup(value);
	params->token = new_token(params->value, params->type, params->state, len);
	params->head = add_token(params->head, params->token);
	if (len > 1)
		params->i++;
}

void	handle_quotes(t_token_params *params, int quote, char *quote_type)
{
	if (quote == 1 && params->in_dquote)
	{
		handle_special_char(params, quote_type, 1);
		return ;
	}
	set_in_quotes(params, quote);
	set_states(params, quote);
	params->value = ft_strdup(quote_type);
	params->type = ft_strdup(quote_type);
	params->token = new_token(params->value, params->type,
			GENERAL, ft_strlen(params->value));
	params->head = add_token(params->head, params->token);
}

void	handle_word(t_token_params *params, char *line)
{
	params->value = get_word(line, &params->i);
	params->type = ft_strdup(WORD);
	params->token = new_token(params->value, params->type,
			params->state, ft_strlen(params->value));
	params->head = add_token(params->head, params->token);
}

void	handle_more_cases(t_token_params *params, char *line)
{
	if (line[params->i] == '>' && line[params->i + 1] == '>')
		handle_special_char(params, APPEND_OUT, 2);
	else if (line[params->i] == '<')
		handle_special_char(params, REDIR_IN, 1);
	else if (line[params->i] == '>')
		handle_special_char(params, REDIR_OUT, 1);
	else if (line[params->i] == '\\' && line[params->i + 1] == 'n')
		handle_special_char(params, NEW_LINE, 2);
	else if (line[params->i] == '\\' && line[params->i + 1] == 't')
		handle_special_char(params, TAB_SPACE, 2);
	else if (line[params->i] == '\\')
		handle_special_char(params, ESCAPE, 1);
	else
		handle_word(params, line);
}

void	handle_cases(t_token_params *params, char *line)
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
	else
		handle_more_cases(params, line);
}
