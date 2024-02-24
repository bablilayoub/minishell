/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:33:23 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/24 02:49:44 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void excute_childs(t_cmd *cmd, int fd[][2], int k, int fd_c, t_data *data)
{
	if (k > 0)
		dup2(fd[k][0], STDIN_FILENO);
	if (cmd->next)
		dup2(fd[k + 1][1], STDOUT_FILENO);
	close_fds(fd, fd_c);
	if (cmd->built_in)
		excute_builtin(cmd, data);
	else
	{
		if (execve(cmd->path, cmd->arguments, data->env) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
}
void close_fds_and_wait(int fd[][2], pid_t pid[], int fd_c, t_data *data)
{
	int i;
	int status;

	status = 0;
	i = -1;
	while (++i < fd_c)
		close(fd[i][1]);
	i = -1;
	while (++i < fd_c)
		 waitpid(pid[i], &status, 0);
	data->exit_status = WEXITSTATUS(status);
	close_fds(fd, fd_c);
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
			excute_childs(current, fd, k, fd_c, data);
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
