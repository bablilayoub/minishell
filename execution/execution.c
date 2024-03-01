/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:33:23 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/01 23:04:28 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void handle_redirections(t_cmd *cmd, int fd[][2], int k, int fd_c)
{
	int fd_in = 0;
	int fd_out = 1;

	if (cmd->has_redir_in || cmd->has_redir_in)
	{
		// Rah 9adi redirects (Double pointer dyal input_file w output_file), ta t3awd 3la had l'partie
		return;
		// if (cmd->redirect_in && !ft_strncmp(cmd->redirect_in, "<<", 2))
		// 	fd_in = open(cmd->input_file, O_RDONLY);
		// else if (cmd->redirect_in)
		// 	fd_in = open(cmd->input_file, O_RDONLY);
		// if (cmd->redirect_out && !ft_strncmp(cmd->redirect_out, ">>", 2))
		// 	fd_out = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		// else if (cmd->redirect_out)
		// 	fd_out = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

void incoming_data(t_cmd *current, t_data *data, int k, pid_t pid[])
{

	if (ft_strncmp(current->cmd, "export", 6) == 0
		|| (ft_strncmp(current->cmd, "unset", 5) == 0))
	{
		waitpid(pid[k], NULL, 0);
		update_env(data, current->cmd);
	}
	else if (ft_strncmp(current->cmd, "cd", 2) == 0)
	{
		waitpid(pid[k], NULL, 0);
		change_path(data);
	}
	else if (ft_strncmp(current->cmd, "exit", 4) == 0)
	{
		waitpid(pid[k], NULL, 0);
		exit_shell();
	}
}

void excute_child(t_cmd *current, t_data *data, int fd[][2], int k, int fd_c)
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
			excute_child(current, data, fd, k, fd_c);
		if (pid[k] > 0)
			incoming_data(current, data, k, pid);
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
