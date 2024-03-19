/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 03:50:22 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/19 21:36:34 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

int	iterate_redirections(t_cmd *cmd, t_redirection *tmp, t_data *data)
{
	while (tmp)
	{
		if (ft_strncmp(tmp->type, "<", ft_strlen(tmp->type)) == 0
			|| ft_strncmp(tmp->type, "<<", ft_strlen(tmp->type)) == 0)
		{
			if (!redirections_in(cmd, &cmd->fd_in, tmp, data))
				return (0);
		}
		else if (ft_strncmp(tmp->type, ">", ft_strlen(tmp->type)) == 0
			|| ft_strncmp(tmp->type, ">>", ft_strlen(tmp->type)) == 0)
		{
			if (!redirections_out(cmd, &cmd->fd_out, tmp, data))
				return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	handle_single_command_redirections(t_cmd *cmd, t_data *data)
{
	t_redirection	*tmp;

	(1) && (cmd->fd_out = STDOUT_FILENO, cmd->fd_in = STDIN_FILENO);
	tmp = cmd->redirects;
	if (cmd->has_redirection)
	{
		if (!iterate_redirections(cmd, tmp, data))
			return (0);
	}
	dup2(cmd->fd_in, STDIN_FILENO);
	dup2(cmd->fd_out, STDOUT_FILENO);
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	return (1);
}
