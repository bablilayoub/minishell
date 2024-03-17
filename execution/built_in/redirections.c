/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 03:49:13 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/17 03:07:59 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

int	redirections_out(t_cmd *cmd, int *fd_out)
{
	t_redirection	*tmp;

	tmp = cmd->redirect_out;
	while (cmd->redirect_out)
	{
		if (*fd_out != STDOUT_FILENO)
			close(*fd_out);
		if (ft_strncmp(cmd->redirect_out->type, ">>", 2) == 0)
			*fd_out = open(cmd->redirect_out->file,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			*fd_out = open(cmd->redirect_out->file,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (*fd_out == -1)
		{
			printf(PREFIX_ERROR "%s: %s\n", cmd->redirect_out->file,
				strerror(errno));
			if (!cmd->built_in)
				exit(EXIT_FAILURE);
			return (0);
		}
		cmd->redirect_out = cmd->redirect_out->next;
	}
	cmd->redirect_out = tmp;
	return (1);
}

int	redirections_in(t_cmd *cmd, int *fd_in)
{
	t_redirection	*tmp;

	tmp = cmd->redirect_in;
	if (tmp)
	{
		while (tmp)
		{
			if (*fd_in != STDIN_FILENO)
				close(*fd_in);
			if (ft_strncmp(tmp->type, "<<", 2) == 0)
				*fd_in = here_doc(tmp->file);
			else
			{
				*fd_in = get_file(cmd, tmp->file);
				if (*fd_in == -1)
					return (0);
			}
			tmp = tmp->next;
		}
	}
	return (1);
}

void	no_redirections(t_cmd **cmd, int k, int fd_c, int **fd)
{
	if ((*cmd)->next)
	{
		if (k == 0)
			(*cmd)->fd_in = STDIN_FILENO;
		else
			(*cmd)->fd_in = fd[k - 1][0];
		if (k == fd_c - 1)
			(*cmd)->fd_out = STDOUT_FILENO;
		else
			(*cmd)->fd_out = fd[k][1];
	}
	else if ((*cmd)->prev)
	{
		if (k == 0)
			(*cmd)->fd_in = STDIN_FILENO;
		else
			(*cmd)->fd_in = fd[k - 1][0];
		(*cmd)->fd_out = STDOUT_FILENO;
	}
	else
	{
		(*cmd)->fd_in = STDIN_FILENO;
		(*cmd)->fd_out = STDOUT_FILENO;
	}
}

int	redir_in_redir_out(t_cmd **cmd, int k, int fd_c, int **fd)
{
	if ((*cmd)->has_redir_in)
	{
		if (!redirections_in(*cmd, &(*cmd)->fd_in))
			return (0);
		if (k == fd_c - 1)
			(*cmd)->fd_out = STDOUT_FILENO;
		else
			(*cmd)->fd_out = fd[k][1];
	}
	if ((*cmd)->has_redir_out)
	{
		if (!redirections_out(*cmd, &(*cmd)->fd_out))
			return (0);
		if (!(*cmd)->has_redir_in)
		{
			if (k == 0)
				(*cmd)->fd_in = STDIN_FILENO;
			else
				(*cmd)->fd_in = fd[k - 1][0];
		}
	}
	return (1);
}

void	handle_redirections(t_cmd *cmd, int **fd, int k, t_data *data)
{
	(1) && (cmd->fd_in = STDIN_FILENO, cmd->fd_out = STDOUT_FILENO);
	if (cmd->has_redir_in || cmd->has_redir_out)
	{
		if (!redir_in_redir_out(&cmd, k, data->fd_c, fd))
		{
			data->exit_status = 1;
			exit(EXIT_FAILURE);
		}
	}
	else
		no_redirections(&cmd, k, data->fd_c, fd);
	dup2(cmd->fd_in, STDIN_FILENO);
	dup2(cmd->fd_out, STDOUT_FILENO);
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
}
