/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:22:17 by abablil           #+#    #+#             */
/*   Updated: 2024/04/21 14:56:50 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	child_handler(int sig)
{
	extern int	g_child_open;

	if (g_child_open && sig == SIGQUIT)
		exit(1);
}

void	signal_handler(int sig)
{
	extern int	g_child_open;

	if (sig == SIGINT)
	{
		if (!g_child_open)
		{
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		else if (g_child_open == 1)
		{
			g_child_open = 2;
			printf("\n");
		}
	}
	if (sig == SIGQUIT)
	{
		if (g_child_open == 1)
			printf("Quit: 3\n");
	}
}
