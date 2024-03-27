/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 03:49:13 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/26 06:25:31 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

int	redirections_out(t_cmd *cmd, int *fd_out, t_redirection *tmp, t_data *data)
{
	if (*fd_out != STDOUT_FILENO)
		close(*fd_out);
	if (ft_strncmp(tmp->type, ">>", 2) == 0)
		*fd_out = open(tmp->file,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		*fd_out = open(tmp->file,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd_out == -1)
	{
		printf(PREFIX_ERROR "%s: %s\n", tmp->file,
			strerror(errno));
		data->exit_status = 1;
		if (!cmd->built_in)
		{
			close_fds(data->fd);
			exit(EXIT_FAILURE);
		}
		return (0);
	}
	return (1);
}

int	redirections_in(t_cmd *cmd, int *fd_in, t_redirection *tmp, t_data *data)
{
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	if (ft_strncmp(tmp->type, "<<", 2) == 0)
		*fd_in = here_doc(tmp, data);
	else
		*fd_in = get_file(cmd, tmp->file);
	if (*fd_in == -1)
	{
		data->exit_status = 1;
		if (!cmd->built_in)
		{
			close_fds(data->fd);
			exit(EXIT_FAILURE);
		}
		return (0);
	}
	return (1);
}

int	redir_in_redir_out(t_cmd *cmd, int k, t_data *data, int **fd)
{
	t_redirection	*tmp;

	tmp = cmd->redirects;
	while (tmp)
	{
		if (ft_strncmp(tmp->type, "<", ft_strlen(tmp->type)) == 0
			|| ft_strncmp(tmp->type, "<<", ft_strlen(tmp->type)) == 0)
		{
			if (!redirections_in(cmd, &cmd->fd_in, tmp, data))
				return (0);
			if (cmd->next)
				cmd->fd_out = fd[k][1];
		}
		else if (ft_strncmp(tmp->type, ">", ft_strlen(tmp->type)) == 0
			|| ft_strncmp(tmp->type, ">>", ft_strlen(tmp->type)) == 0)
		{
			if (!redirections_out(cmd, &cmd->fd_out, tmp, data))
				return (0);
			if (cmd->prev && cmd->fd_in == STDIN_FILENO)
				cmd->fd_in = fd[k - 1][0];
		}
		tmp = tmp->next;
	}
	return (1);
}

void	handle_redirections(t_cmd *cmd, int **fd, int k, t_data *data)
{
	(1) && (cmd->fd_in = STDIN_FILENO, cmd->fd_out = STDOUT_FILENO);
	if (cmd->has_redirection)
	{
		if (!redir_in_redir_out(cmd, k, data, fd))
		{
			data->exit_status = 1;
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (cmd->next)
			cmd->fd_out = fd[k][1];
		if (cmd->prev)
			cmd->fd_in = fd[k - 1][0];
	}
	dup2(cmd->fd_in, STDIN_FILENO);
	dup2(cmd->fd_out, STDOUT_FILENO);
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
}
