/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:33:23 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/21 16:27:08 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void excute_childs(t_cmd *cmd, int fd[][2], int k, int fd_c)
{
    dup2(fd[k][0], STDIN_FILENO);
    dup2(fd[k + 1][1], STDOUT_FILENO);
    close_fds(fd, fd_c);
    if (execve(cmd->path, cmd->arguments, NULL) == -1)
    {
        perror("execv");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
void close_fds_and_wait(int fd[][2], pid_t pid[], int fd_c)
{
    int i;

    i = -1;
    while (++i < fd_c)
        close(fd[i][1]);
    i = -1;
    while (++i < fd_c)
        waitpid(pid[i], NULL, 0);
    close_fds(fd, fd_c);
}
void start_execution(t_cmd *cmd, int fd_c)
{
    int     k;
    int     i;
    int     fd[fd_c][2];
    pid_t   pid[fd_c];
    t_cmd   *current;

    k = 0;
    i = -1;
    current = cmd;
    while (++i < fd_c)
        pipe(fd[i]);
    while (current)
    {
        pid[k] = fork();
        if (pid[k] == -1)
            perror("fork");
        else if (pid[k] == 0)
            excute_childs(current, fd, k, fd_c);
        k++;
        current = current->next;
    }
    close_fds_and_wait(fd, pid, fd_c);
}
void prepare_for_excution(t_cmd *cmd_list)
{
    int fd_c;

    initialize_arguments(cmd_list);
    if (!initialize_path(cmd_list))
        return;
    fd_c = cmd_lenght(cmd_list);
    start_execution(cmd_list, fd_c);
}