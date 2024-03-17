/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 03:50:22 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/17 03:05:50 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

int	handle_single_command_redirections(t_cmd *cmd)
{
	int		fd_out;
	int		fd_in;

	(1) && (fd_out = STDOUT_FILENO, fd_in = STDIN_FILENO);
	if (cmd->has_redir_in || cmd->has_redir_out)
	{
		if (cmd->has_redir_in)
		{
			if (!redirections_in(cmd, &fd_in))
				return (0);
		}
		if (cmd->has_redir_out)
		{
			if (!redirections_out(cmd, &fd_out))
				return (0);
		}
	}
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	if (fd_in != STDIN_FILENO)
		close(fd_in);
	if (fd_out != STDOUT_FILENO)
		close(fd_out);
	return (1);
}
