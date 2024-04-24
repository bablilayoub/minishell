/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:31:53 by abablil           #+#    #+#             */
/*   Updated: 2024/04/23 16:44:49 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*create_new_line(t_data *data)
{
	t_token	*temp;
	char	*new_line;

	temp = data->token;
	new_line = ft_strdup("");
	while (temp)
	{
		data->temp = new_line;
		if (temp && ft_strncmp(temp->type, SPECIAL_CASE,
				ft_strlen(SPECIAL_CASE)) == 0)
			new_line = ft_strjoin(new_line, "''");
		else
			new_line = ft_strjoin(new_line, temp->value);
		free(data->temp);
		temp = temp->next;
	}
	return (new_line);
}

int	parser(char *line, t_data *data)
{
	if (only_spaces(line, data))
		return (0);
	data->token = tokenizer(data, line);
	if (!data->token)
		return (0);
	get_env_vars(data);
	if (!data->token)
		return (0);
	if (!check_quotes(data->token, data))
		return (0);
	if (!convert_tokens_to_commands(data))
		return (0);
	if (!check_syntax(data, line))
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
	free_data(data, 1);
}
