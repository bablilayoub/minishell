/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cases.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 21:18:29 by abablil           #+#    #+#             */
/*   Updated: 2024/03/14 01:08:49 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void handle_special_char(t_token_params *params, char *value, int len)
{
	char *temp;
	int parent_pid;
	int expandable;
	char *reset = NULL;
	
	expandable = 1;
	if (value[0] == '$' && value[1] == '\0' && params->line[params->i + 1] != '$')
	{
		params->i++;
		if (params->line[params->i] >= '1' && params->line[params->i] <= '9')
		{
			expandable = 0;
			params->i++;
		}
		temp = get_word(params->line, &params->i, 1);
		if (temp)
		{
			if (expandable)
				params->value = ft_strjoin(value, temp);
			else
				params->value = ft_strdup(temp);
			free(temp);
		}
		else
		{
			if (params->state == IN_DQUOTE || params->state == IN_QUOTE || params->line[params->i] == '\0')
				params->value = ft_strdup(value);
			else
				params->value = ft_strdup("");

		}
		params->i--;
		len = ft_strlen(params->value);
		if (params->value[0] == '$' && ft_strlen(params->value) == 1)
			params->type = ft_strdup(WORD);
		else
			params->type = ft_strdup(ENV);
	}
	else if (value[0] == '$' && value[1] == '\0' && params->line[params->i + 1] == '$')
	{
		parent_pid = fork();
		if (parent_pid == 0)
			exit(0);
		waitpid(parent_pid, NULL, 0);
		params->value = ft_itoa(parent_pid - 1);
		params->type = ft_strdup(WORD);
		params->i++;
	}
	else if (value[0] == '\\')
	{
		params->i++;
		if (params->line[params->i] && params->line[params->i] != '\0')
		{
			if (params->state == IN_DQUOTE || params->state == IN_QUOTE)
			{
				params->value = ft_strdup("\\");
				reset = get_word(params->line, &params->i, 1);
				if (reset)
				{
					temp = ft_strjoin(params->value, reset);
					free(params->value);
					params->value = temp;
					free(reset);
				}
				params->type = ft_strdup(WORD);
			}
			else
			{
				params->value = ft_strdup("\\");
				params->type = ft_strdup(WORD);
			}
		}
		else if (params->line[params->i] == '\0' && (params->state == IN_DQUOTE || params->state == IN_QUOTE))
		{
			params->value = ft_strdup("\\");
			params->type = ft_strdup(WORD);
		}
		else
		{
			printf(PREFIX_ERROR "syntax error near unexpected token `\\'\n");
			return;
		}
		len = ft_strlen(params->value);
	}
	else
	{
		params->value = ft_strdup(value);
		if (params->state == IN_DQUOTE || params->state == IN_QUOTE)
			params->type = ft_strdup(WORD);
		else
			params->type = ft_strdup(value);
	}
	params->token = new_token(params->value, params->type, params->state, len);
	params->head = add_token(params->head, params->token);
	if (len > 1 && value[0] != '\\')
		params->i++;
}

void handle_quotes(t_token_params *params, int quote, char *quote_type)
{
	if (quote == 1 && params->in_dquote)
	{
		handle_special_char(params, quote_type, 1);
		return;
	}
	else if (quote == 2 && params->in_quote)
	{
		handle_special_char(params, quote_type, 1);
		return;
	}
	set_in_quotes(params, quote);
	set_states(params, quote);
	params->value = ft_strdup(quote_type);
	params->type = ft_strdup(quote_type);
	params->token = new_token(params->value, params->type,
							  GENERAL, ft_strlen(params->value));
	params->head = add_token(params->head, params->token);
}

void handle_word(t_token_params *params, char *line)
{
	params->value = get_word(line, &params->i, 0);
	params->type = ft_strdup(WORD);
	params->token = new_token(params->value, params->type,
							  params->state, ft_strlen(params->value));
	params->head = add_token(params->head, params->token);
}

void handle_more_cases(t_token_params *params, char *line)
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
	else
		handle_more_cases(params, line);
}
