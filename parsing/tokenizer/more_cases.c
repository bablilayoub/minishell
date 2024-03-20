/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_cases.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 03:49:37 by abablil           #+#    #+#             */
/*   Updated: 2024/03/20 00:13:16 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	handle_quotes(t_token_params *params, int quote, char *quote_type)
{
	if (params->state == GENERAL && ((quote == 1 && (params->line[params->i + 1] == '\'')) || (quote == 2 && (params->line[params->i + 1] == '\"'))))
	{
		if (quote == 1 && params->line[params->i + 1] == '\'')
			params->value = ft_strdup("");
		else if (quote == 2 && params->line[params->i + 1] == '\"')
			params->value = ft_strdup("");
		params->type = ft_strdup(SPECIAL_CASE);
		params->token = new_token(params->value, params->type,
				GENERAL, ft_strlen(params->value));
		params->head = add_token(params->head, params->token);
		params->i++;
		return ;
	}
	if (quote == 1 && params->in_dquote)
	{
		handle_special_char(params, quote_type, 1);
		return ;
	}
	else if (quote == 2 && params->in_quote)
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
	params->value = get_word(line, &params->i, 0);
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
	else if (line[params->i] == '\\')
		handle_special_char(params, ESCAPE, 1);
	else
		handle_word(params, line);
}

void	handle_else(t_token_params *params, char *value)
{
	params->value = ft_strdup(value);
	if (params->state == IN_DQUOTE || params->state == IN_QUOTE)
		params->type = ft_strdup(WORD);
	else
		params->type = ft_strdup(value);
}
