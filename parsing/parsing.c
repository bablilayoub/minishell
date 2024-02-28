/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:31:53 by abablil           #+#    #+#             */
/*   Updated: 2024/02/28 16:23:23 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

void reader(t_data *data)
{
	char *line;
	
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	
	rl_catch_signals = 0;
	while (1)
	{
		line = readline(data->prefix);
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
		line = NULL;
	}
	rl_clear_history();
	// free_data(data);
}
