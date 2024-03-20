/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cases.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 21:18:29 by abablil           #+#    #+#             */
/*   Updated: 2024/03/20 22:18:17 by abablil          ###   ########.fr       */
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
	{
		if ((params->line[params->i] == '"' || params->line[params->i] == '\'') && params->state == GENERAL)
			params->value = ft_strdup("");
		else
			params->value = ft_strdup(value);
	}
	(1 == 1) && (params->i--, *len = ft_strlen(params->value));
}

void	handle_double_dollar(t_token_params *params)
{
	int		parent_pid;

	parent_pid = fork();
	if (parent_pid == 0)
		exit(0);
	waitpid(parent_pid, NULL, 0);
	params->value = ft_itoa(parent_pid - 1);
	params->type = ft_strdup(WORD);
	params->i++;
}

int	handle_escape(t_token_params *p, char **temp, char **reset)
{
	p->i++;
	if (p->line[p->i] && p->line[p->i] != '\0')
	{
		if (p->state == IN_DQUOTE || p->state == IN_QUOTE)
		{
			(1 == 1) && (p->value = ft_strdup("\\"), p->type = ft_strdup(WORD));
			*reset = get_word(p->line, &p->i, 1);
			if (*reset)
			{
				*temp = ft_strjoin(p->value, *reset);
				free(p->value);
				p->value = *temp;
				free(*reset);
			}
		}
		else
			(1 == 1) && (p->value = ft_strdup("\\"), p->type = ft_strdup(WORD));
	}
	else if (p->line[p->i] == '\0'
		&& (p->state == IN_DQUOTE || p->state == IN_QUOTE))
		(1 == 1) && (p->value = ft_strdup("\\"), p->type = ft_strdup(WORD));
	else
		return (printf(PREFIX_ERROR "syntax error near token `\\'\n"));
	return (1);
}

void	handle_special_char(t_token_params *params, char *value, int len)
{
	if (value[0] == '$' && value[1] == '\0'
		&& params->line[params->i + 1] != '$')
	{
		params->type = NULL;
		handle_exapndable(params, &params->temp, &len, value);
		if (params->value[0] == '$' && ft_strlen(params->value) == 1 && !params->type)
			params->type = ft_strdup(WORD);
		else if (!params->type)
			params->type = ft_strdup(ENV);
	}
	else if (value[0] == '$' && value[1] == '\0'
		&& params->line[params->i + 1] == '$')
		handle_double_dollar(params);
	else if (value[0] == '\\')
	{
		if (handle_escape(params, &params->temp, &params->reset) != 1)
			return ;
		len = ft_strlen(params->value);
	}
	else
		handle_else(params, value);
	params->token = new_token(params->value, params->type, params->state, len);
	params->head = add_token(params->head, params->token);
	if (len > 1 && value[0] != '\\')
		params->i++;
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
