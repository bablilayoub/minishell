/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 08:51:56 by abablil           #+#    #+#             */
/*   Updated: 2024/03/20 00:04:53 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	*tokenizer(char *line)
{
	t_token_params	params;

	params.i = 0;
	params.state = GENERAL;
	params.head = NULL;
	params.in_quote = 0;
	params.in_dquote = 0;
	params.line = line;
	if (!line)
		return (NULL);
	while (params.i < ft_strlen(line) && line[params.i])
	{
		handle_cases(&params, line);
		params.i++;
	}
	return (params.head);
}
