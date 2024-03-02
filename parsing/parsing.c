/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:31:53 by abablil           #+#    #+#             */
/*   Updated: 2024/03/01 17:04:24 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	parser(char *line, t_data *data)
{
	if (only_spaces(line))
		return (0);
	data->token = tokenizer(line);
	convert_tokens_to_commands(data);
	get_env_vars(data);
	if (!check_syntax(data))
		return (0);
	return (1);
}

void	reader(t_data *data)
{
	char	*line;

	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	rl_catch_signals = 0;
	while (1)
	{
		line = readline(data->prefix);
		if (!line)
			break ;
		if (ft_strlen(line) > 0)
		{
			if (parser(line, data))
				prepare_for_excution(data);
			add_history(line);
		}
		free(line);
		line = NULL;
	}
	rl_clear_history();
}
