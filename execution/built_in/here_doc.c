/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 03:51:23 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/26 06:40:33 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void	here_doc_signal_handler(int sig)
{
	if (sig == SIGINT)
		exit(1);
}

char	*token_to_string(t_token *token, char *line)
{
	char	*new_line;
	char	*tmp;
	t_token	*tmp_token;

	free(line);
	new_line = ft_strdup("");
	check_error_null(new_line, "malloc");
	tmp_token = token;
	while (token)
	{
		tmp = new_line;
		new_line = ft_strjoin(new_line, token->value);
		check_error_null(new_line, "malloc");
		free(tmp);
		token = token->next;
	}
	token = tmp_token;
	free_token(token);
	return (new_line);
}

void	handle_here_doc_env(t_data *data, t_token **token)
{
	if ((*token)->value && (ft_strncmp((*token)->value, "$?", 2) == 0))
		get_exit_status(data, *token);
	else if ((*token)->value
		&& ft_strncmp((*token)->type, ENV, ft_strlen(ENV)) == 0
		&& ft_strlen((*token)->value) > 1
		&& ft_strncmp((*token)->value, "$?", 2) != 0)
		get_normal_env_vars(data, *token);
	else if ((*token)->value && ft_strncmp((*token)->value, "$", 1) == 0)
	{
		data->temp = (*token)->type;
		(*token)->type = ft_strdup(WORD);
		check_error_null((*token)->type, "malloc");
		free(data->temp);
	}
}

char	*expand_env(char *line, t_redirection *tmp, t_data *data)
{
	t_token	*token;
	t_token	*tmp_token;

	if (tmp->expandalbe)
	{
		token = tokenizer(data, line);
		tmp_token = token;
		while (token && tmp->expandalbe)
		{
			handle_here_doc_env(data, &token);
			token = token->next;
		}
		token = tmp_token;
		line = token_to_string(token, line);
	}
	return (line);
}

int	here_doc(t_redirection *tmp, t_data *data)
{
	int		output;
	char	*line;

	signal(SIGINT, here_doc_signal_handler);
	rl_catch_signals = 1;
	output = open("/tmp/here_doc", O_RDWR | O_CREAT | O_TRUNC, 0644);
	while (1)
	{
		line = readline(YELLOW "> " RESET);
		if (!line)
			break ;
		if ((ft_strncmp(line, tmp->file, ft_strlen(line)) == 0)
			&& ft_strlen(line) == ft_strlen(tmp->file))
		{
			free(line);
			break ;
		}
		line = expand_env(line, tmp, data);
		write(output, line, ft_strlen(line));
		write(output, "\n", 1);
		free(line);
	}
	close(output);
	output = open("/tmp/here_doc", O_RDONLY);
	return (output);
}
