/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:33:23 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/07 22:37:05 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int here_doc(char *file_and_search_for)
{
	int fd[2];
	int ret;
	char *line;

	pipe(fd);
	ret = fork();
	if (ret == 0)
	{
		close(fd[0]);
		while (1)
		{
			line = readline(YELLOW "> " RESET);
			if (!line)
				break;
			if (ft_strncmp(line, file_and_search_for, ft_strlen(line)) == 0)
			{
				free(line);
				break;
			}
			write(fd[1], line, ft_strlen(line));
			write(fd[1], "\n", 1);
			free(line);
		}
		close(fd[1]);
		exit(EXIT_SUCCESS);
	}
	else
	{
		close(fd[1]);
		waitpid(ret, NULL, 0);
	}
	return (fd[0]);
}

char *get_next_line(int fd, char **line)
{
	char *buf;
	char *tmp;
	int ret;

	buf = malloc(2);
	if (!buf)
		return (NULL);
	ret = read(fd, buf, 1);
	if (ret == 0)
	{
		free(buf);
		return (NULL);
	}
	buf[1] = '\0';
	*line = ft_strdup(buf);
	while (1)
	{
		ret = read(fd, buf, 1);
		if (ret == 0)
			break;
		buf[1] = '\0';
		tmp = ft_strjoin(*line, buf);
		free(*line);
		*line = tmp;
		if (ft_strncmp(buf, "\n", 1) == 0)
			break;
	}
	free(buf);
	return (*line);
}

void redirections_out(t_cmd *cmd, int *fd_out)
{
	while (cmd->redirect_out)
	{
		if (ft_strncmp(cmd->redirect_out->type, ">>", 2) == 0)
			*fd_out = open(cmd->redirect_out->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			*fd_out = open(cmd->redirect_out->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		cmd->redirect_out = cmd->redirect_out->next;
	}
}

void redirections_in(t_cmd *cmd, int *fd_in, char *tmp_path)
{
	int fd_tmp;
	char *line;

	if (cmd->redirect_in)
	{
		while (cmd->redirect_in)
		{
			if (ft_strncmp(cmd->redirect_in->type, "<<", 2) == 0)
				*fd_in = here_doc(cmd->redirect_in->file);
			else
			{
				*fd_in = open(cmd->redirect_in->file, O_RDONLY);
				fd_tmp = open(tmp_path, O_RDWR | O_CREAT | O_APPEND, 0644);
				while (get_next_line(*fd_in, &line))
				{
					write(fd_tmp, line, ft_strlen(line));
					free(line);
				}
				write(fd_tmp, "\n", 1);
				close(*fd_in);
				close(fd_tmp);
				*fd_in = open("tmp", O_RDONLY);
			}
			cmd->redirect_in = cmd->redirect_in->next;
		}
	}
}
void handle_redirections(t_data *data, t_cmd *cmd, int fd[][2], int k, int fd_c)
{
	int fd_in = 0;
	int fd_out = 1;
	char *tmp_path;

	tmp_path = ft_strjoin(data->shell_path, "/tmp");
	if (cmd->has_redir_in || cmd->has_redir_out)
	{
		if (cmd->has_redir_out)
		{
			redirections_out(cmd, &fd_out);
			if (k == 0)
				fd_in = STDIN_FILENO;
			else
				fd_in = fd[k - 1][0];
		}
		if (cmd->has_redir_in)
		{
			redirections_in(cmd, &fd_in, tmp_path);
			if (k == fd_c - 1)
				fd_out = STDOUT_FILENO;
			else
				fd_out = fd[k][1];
		}
	}
	else
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
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	free(tmp_path);
}

void incoming_data(t_cmd *current, t_data *data, int k, pid_t pid[])
{
	if ((ft_strncmp(current->cmd, "export", 6) == 0) && !current->arguments[1])
	{
		waitpid(pid[k], NULL, 0);
		export_env_to_exp(data->env, &data->export);
		join_double(&data->export, data->in_valid);
		print(data->export);
	}
	else if (ft_strncmp(current->cmd, "export", 6) == 0 || (ft_strncmp(current->cmd, "unset", 5) == 0))
	{
		waitpid(pid[k], NULL, 0);
		update_env(data, current->cmd, current->arguments);
	}
	else if (ft_strncmp(current->cmd, "cd", 2) == 0)
	{
		waitpid(pid[k], NULL, 0);
		change_path(data);
	}
	else if (ft_strncmp(current->cmd, "exit", 4) == 0)
	{
		waitpid(pid[k], NULL, 0);
		exit_shell(data);
	}
}

void excute_child(t_cmd *current, t_data *data, int fd[][2], int k, int fd_c)
{
	handle_redirections(data, current, fd, k, fd_c);
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
	char *tmp_path;
	
	(1 == 1) && (k = 0, i = -1, current = data->cmd);
	tmp_path = ft_strjoin(data->shell_path, "/tmp");
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
	if (access(tmp_path, F_OK) == 0)
		unlink(tmp_path);
	free(tmp_path);
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
