/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:00:51 by abablil           #+#    #+#             */
/*   Updated: 2024/02/24 02:49:48 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int total, char **args, char **env)
{
	t_data data;
	(void)args;

	if (total != 1)
	{
		printf("Error: minishell\n");
		return (1);
	}
	data.env = env;
	data.token = NULL;
	data.cmd = NULL;
	data.exit_status = 0;
	reader(&data);
	return (0);
}
