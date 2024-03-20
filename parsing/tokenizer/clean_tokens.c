/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 23:49:46 by abablil           #+#    #+#             */
/*   Updated: 2024/03/19 23:10:03 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	create_new_head(t_token **head, t_token **new_head)
{
	while (*head)
	{
		if ((ft_strncmp((*head)->type, DOUBLE_QUOTE, 1) == 0
				|| ft_strncmp((*head)->type, QUOTE, 1) == 0
				|| (ft_strlen((*head)->value) == 0 && ft_strncmp((*head)->type, SPECIAL_CASE, 1) != 0))
			&& (*head)->state == GENERAL)
		{
			(*head) = (*head)->next;
			continue ;
		}
		*new_head = add_token(*new_head, new_token(ft_strdup((*head)->value),
					ft_strdup((*head)->type), (*head)->state, (*head)->len));
		(*head) = (*head)->next;
	}
}

void	normal_words(t_token **tmp, t_token **holder, t_data *data)
{
	while (*tmp && (ft_strncmp((*tmp)->type, WORD, 4) == 0))
	{
		if ((*tmp)->next && ft_strncmp((*tmp)->next->type, WORD, 4) == 0)
		{
			*holder = (*tmp)->next;
			data->temp = (*tmp)->value;
			(*tmp)->value = ft_strjoin((*tmp)->value, (*tmp)->next->value);
			free(data->temp);
			(*tmp)->len = ft_strlen((*tmp)->value);
			(*tmp)->next = (*tmp)->next->next;
			free_token(*holder);
		}
		else
			break ;
	}
}

void	empty_white_spaces(t_token **tmp, t_token **holder, t_data *data)
{
	if (ft_strncmp((*tmp)->type, SPECIAL_CASE, 1) == 0)
		return ;
	while (*tmp && (ft_strncmp((*tmp)->type, WHITE_SPACE, 1) != 0
			|| ft_strlen((*tmp)->value) == 0))
	{
		if ((*tmp)->next && ft_strlen((*tmp)->next->value) == 0 && ft_strncmp((*tmp)->next->type, WHITE_SPACE, 1) == 0)
		{
			*holder = (*tmp)->next;
			data->temp = (*tmp)->type;
			(*tmp)->type = ft_strdup((*tmp)->next->type);
			free(data->temp);
			data->temp = (*tmp)->value;
			(*tmp)->value = ft_strjoin((*tmp)->value, (*tmp)->next->value);
			free(data->temp);
			(*tmp)->len = ft_strlen((*tmp)->value);
			(*tmp)->next = (*tmp)->next->next;
			free_token(*holder);
		}
		else
			break ;
	}
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

t_token	*clean_tokens(t_data *data, t_token *head)
{
	t_token	*new_head;
	t_token	*tmp;
	t_token	*holder;

	new_head = NULL;
	tmp = NULL;
	holder = NULL;
	if (!head)
		return (NULL);
	create_new_head(&head, &new_head);
	tmp = new_head;
	while (tmp)
	{
		// printf("tmp->value: '%s'\n", tmp->value);
		// printf("tmp->type: '%s'\n", tmp->type);
		normal_words(&tmp, &holder, data);
		empty_white_spaces(&tmp, &holder, data);
		if (tmp)
			tmp = tmp->next;
	}
	free_tokens(data->token);
	data->token = new_head;
	return (new_head);
}
