/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cases.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 21:18:29 by abablil           #+#    #+#             */
/*   Updated: 2024/04/05 00:02:01 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	handle_exapndable(t_token_params *params,
	char **temp, int *len, char *value)
{
	int		expandable;

	(1 == 1) && (expandable = 1, params->i++);
	if (params->line[params->i] >= '1' && params->line[params->i] <= '9')
	{
		(1 == 1) && (expandable = 0, params->i++);
		params->type = ft_strdup(WORD);
	}
	*temp = get_word(params->line, &params->i, 1);
	if (*temp)
	{
		if (expandable)
			params->value = ft_strjoin(value, *temp);
		else
			params->value = ft_strdup(*temp);
		free(*temp);
	}
	else
		when_no_word(params, value);
	(1 == 1) && (params->i--, *len = ft_strlen(params->value));
}

void	handle_double_dollar(t_token_params *params)
{
	params->value = ft_itoa(params->data->parent_pid);
	params->type = ft_strdup(WORD);
	params->i++;
}

void	handle_special_char(t_token_params *params, char *value, int len)
{
	if (value[0] == '$' && value[1] == '\0'
		&& params->line[params->i + 1] != '$')
	{
		params->type = NULL;
		handle_exapndable(params, &params->temp, &len, value);
		if (params->value[0] == '$'
			&& ft_strlen(params->value) == 1 && !params->type)
			params->type = ft_strdup(WORD);
		else if (!params->type)
			params->type = ft_strdup(ENV);
	}
	else if (value[0] == '$' && value[1] == '\0'
		&& params->line[params->i + 1] == '$')
		handle_double_dollar(params);
	else
		handle_else(params, value);
	finalize_token(params, len, value);
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
