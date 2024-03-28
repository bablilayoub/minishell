/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:31:53 by abablil           #+#    #+#             */
/*   Updated: 2024/03/28 06:14:39 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	parser(char *line, t_data *data)
{
	if (only_spaces(line, data))
		return (0);
	data->token = tokenizer(data, line);
	get_env_vars(data);
	if (!check_quotes(data->token, data))
		return (0);
	if (!convert_tokens_to_commands(data))
		return (0);
	if (!check_syntax(data))
		return (0);
	return (1);
}

void	reader(t_data *data)
{
	char		*line;
	extern int	g_child_open;

	rl_catch_signals = 1;
	while (1)
	{
		rl_catch_signals = 0;
		line = readline(data->prefix);
		if (!line)
			break ;
		if (ft_strlen(line) > 0)
		{
			if (parser(line, data))
				prepare_for_excution(data);
			g_child_open = 0;
			free_data(data, 0);
			add_history(line);
		}
		free(line);
		line = NULL;
	}
	if (line)
		free(line);
	rl_clear_history();
	free_data(data, 1);
}
