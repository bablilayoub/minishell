/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 23:49:46 by abablil           #+#    #+#             */
/*   Updated: 2024/03/21 00:11:51 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	create_new_head(t_token **head, t_token **new_head)
{
	while (*head)
	{
		if ((ft_strncmp((*head)->type, DOUBLE_QUOTE, 1) == 0
				|| ft_strncmp((*head)->type, QUOTE, 1) == 0
				|| (ft_strlen((*head)->value) == 0
					&& ft_strncmp((*head)->type, SPECIAL_CASE, 1) != 0))
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
		if ((*tmp)->next && ft_strlen((*tmp)->next->value) == 0
			&& ft_strncmp((*tmp)->next->type, WHITE_SPACE, 1) == 0)
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
		normal_words(&tmp, &holder, data);
		empty_white_spaces(&tmp, &holder, data);
		if (tmp)
			tmp = tmp->next;
	}
	free_tokens(data->token);
	data->token = new_head;
	return (new_head);
}
