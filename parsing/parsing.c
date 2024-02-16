/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:31:53 by abablil           #+#    #+#             */
/*   Updated: 2024/02/16 16:34:12 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	reader(void)
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
		if (ft_strlen(line) > 0)
			add_history(line);
		free(line);
	}
}

