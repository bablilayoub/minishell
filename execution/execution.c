/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:33:23 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/26 01:38:22 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void handle_redirections(t_cmd *cmd, int fd[][2], int k, int fd_c)
{
	int fd_in = 0;
	int fd_out = 1;

	if (cmd->redirect_in || cmd->redirect_out)
	{
		if (cmd->redirect_in && !ft_strncmp(cmd->redirect_in, "<<", 2))
			fd_in = open(cmd->input_file, O_RDONLY);
		else if (cmd->redirect_in)
			fd_in = open(cmd->input_file, O_RDONLY);
		if (cmd->redirect_out && !ft_strncmp(cmd->redirect_out, ">>", 2))
			fd_out = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (cmd->redirect_out)
			fd_out = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

void update_env(t_data *data)
{
	int fd_in;
	int bytes;
	char *env;
	char *buff;

	env = ft_strdup("");
	buff = malloc(sizeof(char) * 1024);
	fd_in = open("export.txt", O_RDONLY);
	bytes = 1;
	while (bytes != 0)
	{
		bytes = read(fd_in, buff, 1024);
		if (bytes > 0)
		{
			buff[bytes] = '\0';
			env = ft_strjoin(env, buff);
		}
		else
			break;
	}
	unlink("export.txt");
	data->env = ft_split(env, '\n');
	close(fd_in);
}
char **update_envp(char **env, char *oldpwd, char *pwd)
{
	int i;
	char **new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], "OLDPWD", 6) == 0)
			new_env[i] = ft_strjoin("OLDPWD=", oldpwd);
		else if (ft_strncmp(env[i], "PWD", 3) == 0)
			new_env[i] = ft_strjoin("PWD=", pwd);
		else
			new_env[i] = ft_strdup(env[i]);
	}
	new_env[i] = NULL;
	return (new_env);
}
void change_path(t_data *data, char *path)
{
	char *oldpwd;
	char *pwd;
	int fd_in;

	(void)data;
	fd_in = open("cd.txt", O_RDONLY);
	path = malloc(sizeof(char) * 1024);
	read(fd_in, path, 1024);
	close(fd_in);
	unlink("cd.txt");
	oldpwd = getcwd(NULL, 0);
	chdir(path);
	pwd = getcwd(NULL, 0);
	data->env = update_envp(data->env, oldpwd, pwd);
	// free(oldpwd);
	// free(pwd);
	// free(path);
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
		if (pid[k] > 0)
		{
			if (ft_strncmp(current->cmd, "export", 6) == 0)
			{
				waitpid(pid[k], NULL, 0);
				update_env(data);
			}
			else if (ft_strncmp(current->cmd, "cd", 2) == 0)
			{
				waitpid(pid[k], NULL, 0);
				change_path(data, current->arguments[1]);
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
