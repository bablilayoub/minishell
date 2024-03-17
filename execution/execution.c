/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:33:23 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/17 22:36:38 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	excute_child(t_cmd *current, t_data *data, int k, int fd_c)
{
	handle_redirections(current, data->fd, k, data);
	close_fds(data->fd, fd_c);
	if (current->built_in)
	{
		excute_builtin(current, data);
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (!current->path)
			exit(EXIT_SUCCESS);
		execve(current->path, current->arguments, data->env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

void	excute_multiple_commands(t_data *data, int fd_c,
pid_t pid[], t_cmd *current)
{
	int	k;
	int	i;

	(1) && (k = 0, i = -1);
	while (++i < fd_c)
	{
		if (pipe(data->fd[i]) == -1)
			perror("pipe");
	}
	while (current)
	{
		pid[k] = fork();
		if (pid[k] == -1)
			perror("fork");
		if (pid[k] == 0)
			excute_child(current, data, k, fd_c);
		else
		{
			close(data->fd[k][1]);
			waitpid(pid[k], &data->exit_status, 0);
		}
		k++;
		current = current->next;
	}
	close_fds_and_getstatus(fd_c, data);
}

void	start_execution(t_data *data, int fd_c)
{
	int		i;
	pid_t	*pid;

	if (data->cmd && !data->cmd->next && data->cmd->built_in)
	{
		(1) && (data->in = dup(STDIN_FILENO), data->out = dup(STDOUT_FILENO));
		if (handle_single_command_redirections(data->cmd))
			excute_builtin(data->cmd, data);
		dup2(data->in, STDIN_FILENO);
		dup2(data->out, STDOUT_FILENO);
		(1) && (close(data->in), close(data->out), data->in = 0, data->out = 0);
		return ;
	}
	(1) && (i = -1, pid = malloc(sizeof(pid_t) * fd_c));
	check_error_null(pid, "malloc");
	data->fd = malloc(sizeof(int *) * (fd_c + 1));
	check_error_null(data->fd, "malloc");
	while (++i < fd_c)
	{
		data->fd[i] = malloc(sizeof(int) * 2);
		check_error_null(data->fd[i], "malloc");
	}
	data->fd[i] = NULL;
	excute_multiple_commands(data, fd_c, pid, data->cmd);
	(1) && (free_int(data->fd), free(pid), pid = NULL);
}

void	prepare_for_excution(t_data *data)
{
	t_cmd	*cmd_list;

	cmd_list = data->cmd;
	initialize_arguments(cmd_list);
	if (!initialize_path(cmd_list, data))
		return ;
	data->fd_c = cmd_lenght(cmd_list);
	start_execution(data, data->fd_c);
}
