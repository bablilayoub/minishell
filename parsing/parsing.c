/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:31:53 by abablil           #+#    #+#             */
/*   Updated: 2024/02/16 19:15:29 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_token *new_token(char *value, char *type, int state, size_t len)
{
	t_token *token;

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

t_token *add_token(t_token *head, t_token *token)
{
	t_token *tmp;

	if (!head)
		return (token);
	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = token;
	token->prev = tmp;
	return (head);
}

char *get_word(char *line, size_t *i)
{
	int start;
	char *word;

	start = *i;
	while (line[*i] && ft_isalnum(line[*i]))
		(*i)++;
	word = ft_substr(line, start, *i - start);
	if (!word)
		return (NULL);
	return (word);
}

void *tokenizer(char *line)
{
	t_token *head;
	t_token *token;
	int state;
	size_t i;
	char *value;
	char *type;

	i = 0;
	state = GENERAL;
	head = NULL;
	while (i < ft_strlen(line))
	{
		if (ft_isalnum(line[i]))
		{
			value = get_word(line, &i);
			type = ft_strdup(WORD);
			token = new_token(value, type, state, ft_strlen(value));
			head = add_token(head, token);
		}
		i++;
	}
	return (head);
}

void parser(char *line, t_data *data)
{
	(void)data;

	if (only_spaces(line))
		return;

	// Tokenize
	data->token = tokenizer(line);
	printf("Tokens:\n");
	printf("-----------------------------------------------------------------\n");
	printf("|	value	|	type	|	state	|	len	|\n");
	printf("-----------------------------------------------------------------\n");
	while (data->token)
	{
		char *state;
		// change values
		switch (data->token->state)
		{
			case 0:
				state = "GENERAL";
				break;
			case 1:
				state = "IN_QUOTE";
				break;
			case 2:
				state = "IN_DQUOTE";
				break;
		}
		
		printf("|	%s	|	%s	|	%s	|	%zu	|\n", data->token->value, data->token->type, state, data->token->len);
		data->token = data->token->next;
	}
	printf("-----------------------------------------------------------------\n");
}

void reader(t_data *data)
{
	char *line;

	while (1)
	{
		line = readline(PREFIX);
		if (!line)
		{
			clear_history();
			break;
		}
		parser(line, data);
		if (ft_strlen(line) > 0)
			add_history(line);
		free(line);
	}
}
