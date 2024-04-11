/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 13:07:27 by abablil           #+#    #+#             */
/*   Updated: 2024/04/11 13:11:26 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int	handle_first_pipe(int *first_pipe, t_token **tmp)
{
	if (*first_pipe == 0)
		*first_pipe = 1;
	if (*first_pipe == 1)
	{
		*tmp = (*tmp)->prev;
		while (*tmp && ft_strncmp(WHITE_SPACE, (*tmp)->type, 1) == 0)
			*tmp = (*tmp)->prev;
	}
	else
	{
		*tmp = (*tmp)->next;
		while (*tmp && ft_strncmp(WHITE_SPACE, (*tmp)->type, 1) == 0)
			*tmp = (*tmp)->next;
	}
	if (!*tmp)
	{
		printf("%s\n", PREFIX_ERROR "syntax error, pipe not closed");
		return (0);
	}
	return (1);
}

int	check_pipe_syntax(t_token *tmp)
{
	if (ft_strncmp(tmp->type, WORD, ft_strlen(tmp->type)) != 0
		&& ft_strncmp(tmp->type, ENV, ft_strlen(tmp->type)) != 0
		&& ft_strncmp(tmp->type, SPECIAL_CASE, ft_strlen(tmp->type)) != 0
		&& ft_strncmp(tmp->type, APPEND_OUT, ft_strlen(tmp->type)) != 0
		&& ft_strncmp(tmp->type, REDIR_OUT, ft_strlen(tmp->type)) != 0
		&& ft_strncmp(tmp->type, REDIR_IN, ft_strlen(tmp->type)) != 0
		&& ft_strncmp(tmp->type, HERE_DOC, ft_strlen(tmp->type)) != 0
		&& ft_strncmp(tmp->type, QUOTE, ft_strlen(tmp->type)) != 0
		&& ft_strncmp(tmp->type, DOUBLE_QUOTE, 1) != 0)
	{
		printf("%s\n", PREFIX_ERROR "syntax error, pipe not closed\n");
		return (0);
	}
	return (1);
}

int	check_pipes(char *line, t_data *data)
{
	t_token	*tokens;
	t_token	*tmp;
	int		first_pipe;

	(1) && (tokens = tokenizer(data, line), tmp = tokens, first_pipe = 0);
	while (tmp)
	{
		if (ft_strncmp(tmp->type, PIPE_LINE, 1) == 0 && tmp->state == GENERAL)
		{
			if (!handle_first_pipe(&first_pipe, &tmp))
			{
				free_tokens(tokens);
				return (0);
			}
			if (!check_pipe_syntax(tmp))
			{
				free_tokens(tokens);
				return (0);
			}
			first_pipe = 2;
		}
		tmp = tmp->next;
	}
	free_tokens(tokens);
	return (1);
}
