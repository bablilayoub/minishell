/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:23:16 by abablil           #+#    #+#             */
/*   Updated: 2024/04/11 17:39:45 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int	check_quotes(t_token *token, t_data *data)
{
	t_token	*tmp;
	int		count;

	count = 0;
	tmp = token;
	while (tmp)
	{
		if ((ft_strncmp(tmp->type, QUOTE, 1) == 0
				|| ft_strncmp(tmp->type, DOUBLE_QUOTE, 1) == 0)
			&& tmp->state == GENERAL)
			count++;
		tmp = tmp->next;
	}
	if (count % 2 != 0)
	{
		data->exit_status = 258;
		printf(PREFIX_ERROR "syntax error, quotes not closed\n");
		return (0);
	}
	return (1);
}

int	check_redirections(t_cmd *cmd, t_data *data)
{
	t_cmd			*tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->has_redirection)
		{
			if (!validate_redirects(&tmp->redirects, data))
				return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	check_ambiguous_redirects(char *line, t_data *data)
{
	t_token	*tokens;
	t_token	*tmp;
	char	*file;

	(1) && (tokens = tokenizer(data, line), tmp = tokens, file = NULL);
	while (tmp)
	{
		while (tmp && not_a_shell_command(tmp))
			tmp = tmp->next;
		if (tmp && is_redirect(tmp) && tmp->state == GENERAL)
		{
			tmp = tmp->next;
			while (tmp && is_whitespace_or_quotes(tmp) && tmp->state == GENERAL)
				tmp = tmp->next;
			if (tmp && ft_strncmp(tmp->type, ENV, 1) == 0)
			{
				if (!handle_env(tmp, &file, data, tokens))
					return (0);
			}
		}
		if (tmp)
			tmp = tmp->next;
	}
	free_tokens(tokens);
	return (1);
}

int	check_syntax(t_data *data, char *line)
{
	if (line)
	{
		if (!check_ambiguous_redirects(line, data))
		{
			data->exit_status = 1;
			return (0);
		}
	}
	if (!check_redirections(data->cmd, data))
	{
		data->exit_status = 258;
		return (0);
	}
	if (line)
	{
		if (!check_pipes(line, data))
		{
			data->exit_status = 258;
			return (0);
		}
	}
	return (1);
}
