/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:31:53 by abablil           #+#    #+#             */
/*   Updated: 2024/02/16 12:33:10 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parser(char *line, t_data *data)
{
	(void)data;

	
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (strcmp(line, "pwd") == 0)
		{
			printf("pwd\n");
		}
	}
}