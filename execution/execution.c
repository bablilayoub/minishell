/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:33:23 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/19 12:07:05 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void excute_childs(t_cmd *cmd, int fd[][2], int k, int cmd_count)
{
    dup2(fd[k][0], STDIN_FILENO);
    dup2(fd[k + 1][1], STDOUT_FILENO);
    close_fds(fd, cmd_count);
    execve(cmd->path, cmd->arguments, NULL);
    perror("execv");
    exit(EXIT_FAILURE);
}
void start_execution(t_cmd *cmd, int fd_c, int cmd_count)
{
    int fd[fd_c][2];
    pid_t pid[cmd_count];
    t_cmd *current_cmd = cmd;
    int k;
    int i;

    k = 0;
    i = -1;

    while (++i < fd_c)
        pipe(fd[i]);

    while (current_cmd != NULL)
    {
        pid[k] = fork();
        if (pid[k] == -1)
            perror("fork");
        else if (pid[k] == 0)
        {
            excute_childs(current_cmd, fd, k, cmd_count);
            exit(EXIT_FAILURE);
        }
        k++;
        current_cmd = current_cmd->next;
    }

    exit(EXIT_SUCCESS);
    i = -1;
    while (++i < cmd_count)
        close(fd[i][1]);
    i = -1;
    while (++i < cmd_count)
        waitpid(pid[i], NULL, 0);

    char buff[10];
    int bytes_read;
    while ((bytes_read = read(fd[k - 1][0], buff, sizeof(buff))) > 0)
    {
        write(STDOUT_FILENO, buff, bytes_read);
    }
    close_fds(fd, cmd_count);
    if (bytes_read == -1)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }
}

void prepare_for_excution()
{
    int fd_c;
    t_cmd *cmd_list;

    cmd_list = parsed_list();
    fd_c = cmd_lenght(cmd_list) - 1;
    start_execution(cmd_list, fd_c, fd_c + 1);
}