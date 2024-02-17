/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 08:57:01 by abablil           #+#    #+#             */
/*   Updated: 2024/02/17 08:57:32 by abablil          ###   ########.fr       */
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
	size_t start;
	char *word;

	start = *i;
	while (line[*i] && ft_isalnum(line[*i]))
		(*i)++;
	word = ft_substr(line, start, *i - start);
	if (!word)
		return (NULL);
	if (line[*i] != '\0')
		(*i)--;
	return (word);
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
		space_left = 14 - ft_strlen(tmp->type);
		printf("|    %s", tmp->type);
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