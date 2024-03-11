/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:33:23 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/11 00:36:26 by abablil          ###   ########.fr       */
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

void redirections_in(t_cmd *cmd, int *fd_in)
{
	if (cmd->redirect_in)
	{
		while (cmd->redirect_in)
		{
			if (ft_strncmp(cmd->redirect_in->type, "<<", 2) == 0)
				*fd_in = here_doc(cmd->redirect_in->file);
			else
				*fd_in = open(cmd->redirect_in->file, O_RDONLY);
			cmd->redirect_in = cmd->redirect_in->next;
		}
	}
}

void no_redirections(t_cmd *cmd, int *fd_in, int *fd_out, int fd[][2], int k, int fd_c)
{
	if (cmd->next)
	{
		if (k == 0)
			*fd_in = STDIN_FILENO;
		else
			*fd_in = fd[k - 1][0];
		if (k == fd_c - 1)
			*fd_out = STDOUT_FILENO;
		else
			*fd_out = fd[k][1];
	}
	else if (cmd->prev)
	{
		if (k == 0)
			*fd_in = STDIN_FILENO;
		else
			*fd_in = fd[k - 1][0];
		*fd_out = STDOUT_FILENO;
	}
	else
	{
		*fd_in = STDIN_FILENO;
		*fd_out = STDOUT_FILENO;
	}
}
void handle_redirections(t_cmd *cmd, int fd[][2], int k, int fd_c)
{
	int fd_in = 0;
	int fd_out = 1;

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
			redirections_in(cmd, &fd_in);
			if (k == fd_c - 1)
				fd_out = STDOUT_FILENO;
			else
				fd_out = fd[k][1];
		}
	}
	else
		no_redirections(cmd, &fd_in, &fd_out, fd, k, fd_c);
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
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
		if (!current->path)
			exit(EXIT_SUCCESS);
		execve(current->path, current->arguments, data->env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

void handle_single_command_redirections(t_cmd *cmd)
{
	int fd_out = 1;
	int fd_in = 0;
	int flag = 0;

	if (cmd->has_redir_in || cmd->has_redir_out)
	{
		if (cmd->has_redir_out)
		{
			redirections_out(cmd, &fd_out);
			flag = 1;
		}
		if (cmd->has_redir_in)
		{
			redirections_in(cmd, &fd_in);
			flag = 2;
		}
	}
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	if (flag == 1)
		close(fd_out);
	if (flag == 2)
		close(fd_in);
}
void start_execution(t_data *data, int fd_c)
{
	int k;
	int i;
	int fd[fd_c][2];
	pid_t pid[fd_c];
	t_cmd *current;
	int tmp_in;
	int tmp_out;

	k = 0;
	i = -1;
	current = data->cmd;
	if (current && !current->next && current->built_in)
	{
		tmp_in = dup(STDIN_FILENO);
		tmp_out = dup(STDOUT_FILENO);
		handle_single_command_redirections(current);
		excute_builtin(current, data);
		dup2(tmp_in, STDIN_FILENO);
		dup2(tmp_out, STDOUT_FILENO);
		close(tmp_in);
		close(tmp_out);
		return;
	}
	while (++i < fd_c)
		pipe(fd[i]);
	while (current)
	{
		pid[k] = fork();
		if (pid[k] == 0)
			excute_child(current, data, fd, k, fd_c);
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
