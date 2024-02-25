/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:33:23 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/25 04:37:15 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void handle_redirections(t_cmd *cmd, int fd[][2], int k, int fd_c)
{
	int fd_in = 0;
	int fd_out = 1;

	if (cmd->redirect_in || cmd->redirect_out || cmd->here_doc || cmd->append_out)
	{
		if (cmd->redirect_in)
			fd_in = open(cmd->input_file, O_RDONLY);
		if (cmd->redirect_out)
			fd_out = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (cmd->here_doc)
			fd_in = open(cmd->input_file, O_RDONLY);
		if (cmd->append_out)
			fd_out = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
	{
		if (cmd->next)
		{
			if (k == 0)
				fd_in = STDIN_FILENO;
			else
				fd_in = fd[k - 1][0];
			if (k == fd_c - 1)
				fd_out = STDOUT_FILENO;
			else
				fd_out = fd[k][1];
		}
		else if (cmd->prev)
		{
			if (k == 0)
				fd_in = STDIN_FILENO;
			else
				fd_in = fd[k - 1][0];
			fd_out = STDOUT_FILENO;
		}
		else
		{
			fd_in = STDIN_FILENO;
			fd_out = STDOUT_FILENO;
		}
	}
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
}
void close_fds_and_wait(int fd[][2], pid_t pid[], int fd_c, t_data *data)
{
	int i;
	int status;

	status = 0;
	close_fds(fd, fd_c);
	i = -1;
	while (++i < fd_c)
		waitpid(pid[i], &status, 0);
	data->exit_status = WEXITSTATUS(status);
}
void start_execution(t_data *data, int fd_c)
{
	int k;
	int i;
	int fd[fd_c][2];
	pid_t pid[fd_c];
	t_cmd *current;

	k = 0;
	i = -1;
	current = data->cmd;
	while (++i < fd_c)
		pipe(fd[i]);
	while (current)
	{

		pid[k] = fork();
		if (pid[k] == 0)
		{
			handle_redirections(current, fd, k, fd_c);
			close_fds(fd, fd_c);
			if (current->built_in)
			{
				excute_builtin(current, data);
				exit(EXIT_SUCCESS);
			}
			else
			{
				execve(current->path, current->arguments, data->env);
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		k++;
		current = current->next;
	}
	close_fds_and_wait(fd, pid, fd_c, data);
}

void prepare_for_excution(t_data *data)
{
	int fd_c;
	t_cmd *cmd_list;

	cmd_list = data->cmd;
	initialize_arguments(cmd_list);
	if (!initialize_path(cmd_list, data))
		return;
	fd_c = cmd_lenght(cmd_list);
	start_execution(data, fd_c);
}
