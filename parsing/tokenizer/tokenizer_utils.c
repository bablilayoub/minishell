/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 08:57:01 by abablil           #+#    #+#             */
/*   Updated: 2024/03/09 02:52:34 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

t_token	*new_token(char *value, char *type, int state, size_t len)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
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
	{
		while (line[*i] && ft_isalnum(line[*i]))
			(*i)++;
	}
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
	while (tmp && ft_strncmp(tmp->type, WHITE_SPACE, 1) == 0)
		tmp = tmp->next;
	return (tmp);
}

void print_tokens(t_token *token)
{
	t_token *tmp;
	int i;
	int space_left;

	tmp = token;
	printf("-------------------------------------------------------------------------------\n");
	printf("|    Value           |    Lenght        |    State         |    Type          |\n");
	printf("-------------------------------------------------------------------------------\n");
	while (tmp)
	{
		i = 0;
		space_left = 14 - ft_strlen(tmp->value);
		printf("|    '%s'", tmp->value);
		while (i < space_left)
		{
			printf(" ");
			i++;
		}
		i = 0;
		space_left = 14 - ft_strlen(ft_itoa(tmp->len));
		printf("|    %zu", tmp->len);
		while (i < space_left)
		{
			printf(" ");
			i++;
		}
		i = 0;
		switch (tmp->state)
		{
		case 0:
			printf("|    GENERAL");
			break;
		case 1:
			printf("|    IN_QUOTE");
			break;
		case 2:
			printf("|    IN_DQUOTE");
			break;
		}
		switch (tmp->state)
		{
		case 0:
			space_left = 14 - ft_strlen("GENERAL");
			break;
		case 1:
			space_left = 14 - ft_strlen("IN_QUOTE");
			break;
		case 2:
			space_left = 14 - ft_strlen("IN_DQUOTE");
			break;
		}
		while (i < space_left)
		{
			printf(" ");
			i++;
		}
		i = 0;
		char *new_type;
		if (ft_strncmp(tmp->type, QUOTE, 6) == 0)
			new_type = "QUOTE";
		else if (ft_strncmp(tmp->type, DOUBLE_QUOTE, 6) == 0)
			new_type = "DOUBLE_QUOTE";
		else if (ft_strncmp(tmp->type, WORD, 4) == 0)
			new_type = "WORD";
		else if (ft_strncmp(tmp->type, WHITE_SPACE, 1) == 0)
			new_type = "WHITE_SPACE";
		else if (ft_strncmp(tmp->type, NEW_LINE, 1) == 0)
			new_type = "NEW_LINE";
		else if (ft_strncmp(tmp->type, ESCAPE, 1) == 0)
			new_type = "ESCAPE";
		else if (ft_strncmp(tmp->type, ENV, 1) == 0)
			new_type = "ENV";
		else if (ft_strncmp(tmp->type, PIPE_LINE, 1) == 0)
			new_type = "PIPE_LINE";
		else if (ft_strncmp(tmp->type, REDIR_IN, 1) == 0)
			new_type = "REDIR_IN";
		else if (ft_strncmp(tmp->type, REDIR_OUT, 1) == 0)
			new_type = "REDIR_OUT";
		else if (ft_strncmp(tmp->type, HERE_DOC, 2) == 0)
			new_type = "HERE_DOC";
		else if (ft_strncmp(tmp->type, APPEND_OUT, 2) == 0)
			new_type = "APPEND_OUT";
		else if (ft_strncmp(tmp->type, TAB_SPACE, 1) == 0)
			new_type = "TAB_SPACE";
		else
			new_type = tmp->type;
		space_left = 14 - ft_strlen(new_type);
		printf("|    %s", new_type);
		while (i < space_left)
		{
			printf(" ");
			i++;
		}
		printf("|\n");
		tmp = tmp->next;
	}
	printf("-------------------------------------------------------------------------------\n");
}
