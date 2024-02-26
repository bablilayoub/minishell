/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:57:17 by abablil           #+#    #+#             */
/*   Updated: 2024/02/26 17:55:09 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool only_spaces(char *str)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			count++;
		i++;
	}
	if (count == i)
		return (true);
	return (false);
}

t_token *skip_white_spaces(t_token *token)
{
	if (!token)
		return (NULL);
	t_token *tmp = token;
	while (tmp && ft_strncmp(tmp->type, WHITE_SPACE, 1) == 0)
		tmp = tmp->next;
	return (tmp);
}