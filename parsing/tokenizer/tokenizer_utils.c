/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 08:57:01 by abablil           #+#    #+#             */
/*   Updated: 2024/03/21 02:21:35 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

t_token	*new_token(char *value, char *type, int state, size_t len)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	check_error_null(token, "malloc");
	token->value = value;
	token->type = type;
	token->state = state;
	token->len = len;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

t_token	*add_token(t_token *head, t_token *token)
{
	t_token	*tmp;

	if (!head)
		return (token);
	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = token;
	token->prev = tmp;
	return (head);
}

int	not_a_special_char(char c)
{
	if (c != ' ' && c != '\t' && c != '\n' && c != '\'' && c
		!= '\"' && c != '$' && c != '|' && c != '<' && c != '>' && c != '\\')
		return (1);
	return (0);
}

char	*get_word(char *line, size_t *i, int alpha_num)
{
	size_t	start;
	char	*word;

	start = *i;
	if (!line)
		return (NULL);
	if (alpha_num)
		while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
			(*i)++;
	else
		while (line[*i] && not_a_special_char(line[*i]))
			(*i)++;
	if (*i == start)
	{
		if (line[*i] && line[*i] == '?')
			(*i)++;
		else
			return (NULL);
	}
	word = ft_substr(line, start, *i - start);
	if (!word)
		return (NULL);
	if (line[*i] != '\0')
		(*i)--;
	return (word);
}

t_token	*skip_white_spaces(t_token *token)
{
	t_token	*tmp;

	if (!token)
		return (NULL);
	tmp = token;
	while (tmp)
	{
		if (ft_strncmp(tmp->type, SPECIAL_CASE, 1) == 0 && tmp->state == GENERAL
			&& ((tmp->next && ft_strncmp(tmp->next->type, WORD, 4) == 0)
				|| (tmp->prev
					&& ft_strncmp(tmp->prev->type, WORD, 4) == 0)))
		{
			tmp = tmp->next;
			continue ;
		}
		if (ft_strncmp(tmp->type, WHITE_SPACE, 1) != 0)
			break ;
		if (ft_strncmp(tmp->type, SPECIAL_CASE, 1) == 0 && tmp->state == GENERAL
			&& ((tmp->next && ft_strncmp(tmp->next->type, WHITE_SPACE, 1) == 0)
				|| (tmp->prev
					&& ft_strncmp(tmp->prev->type, WHITE_SPACE, 1) == 0)))
			break ;
		tmp = tmp->next;
	}
	return (tmp);
}
