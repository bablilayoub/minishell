/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:31:53 by abablil           #+#    #+#             */
/*   Updated: 2024/02/24 03:35:01 by abablil          ###   ########.fr       */
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
	validate_syntax(data->token);
	check_pipes(data->cmd);
}

void parser(char *line, t_data *data)
{
	if (only_spaces(line))
		return;

	data->token = tokenizer(line);
	convert_tokens_to_commands(data);
	get_env_vars(data);
	check_syntax(data);
	// print_tokens(data->token);
}

void signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

void reader(t_data *data)
{
	char *line;
	
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	
	rl_catch_signals = 0;
	while (1)
	{
		line = readline(PREFIX);
		if (!line)
			break;
		// printf("'line' : '%s'\n", line);
		if (ft_strlen(line) > 0)
		{
			parser(line, data);
			prepare_for_excution(data);
			// free_data(data);
			add_history(line);
		}
		free(line);
	}
	rl_clear_history();
	// free_data(data);
}
