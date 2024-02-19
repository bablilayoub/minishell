/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:33:23 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/19 16:17:26 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void excute_childs(t_cmd *cmd, int fd[][2], int k, int fd_c)
{
    dup2(fd[k][0], STDIN_FILENO);
    dup2(fd[k + 1][1], STDOUT_FILENO);        
    close_fds(fd, fd_c);
    execve(cmd->path, cmd->arguments, NULL);
    perror("execv");
    exit(EXIT_FAILURE);
}
void start_execution(t_cmd *cmd, int fd_c, int cmd_count)
{
    int fd[fd_c][2];
    pid_t pid[cmd_count];
    t_cmd *current = cmd;
    int k;
    int i;

    k = 0;
    i = -1;
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
    i = -1;
    while (++i < fd_c)
        close(fd[i][1]);

    i = -1;
    while (++i < cmd_count)
        waitpid(pid[i], NULL, 0);
    char buff[10];
    int bytes_read;
    while ((bytes_read = read(fd[k][0], buff, sizeof(buff))) > 0)
    {
        if (bytes_read == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        write(STDOUT_FILENO, buff, bytes_read);
    }
    close_fds(fd, fd_c);
}

void  initialize_arguments(t_cmd *cmd_list)
{
    int i;
    t_cmd *temp;

    temp = cmd_list;
    while (cmd_list != NULL)
    {
        cmd_list->arguments = malloc(sizeof(char *) * args_lenght(cmd_list->args) + 1);
        i = 0;
        while (cmd_list->args)
        {
            cmd_list->arguments[i++] = cmd_list->args->arg;
            cmd_list->args = cmd_list->args->next;
        }
        cmd_list->arguments[i] = NULL;
        cmd_list = cmd_list->next;
    }
    
    cmd_list = temp;
    i = 0;
    while (cmd_list != NULL)
    {
        printf("cmd : %s", cmd_list->cmd);
        while (cmd_list->arguments[i])
            printf("Arg : '%s'\n", cmd_list->arguments[i++]);
        cmd_list = cmd_list->next;
    }
    
}
void prepare_for_excution(t_cmd *cmd_list)
{
    int fd_c;
    
    initialize_arguments(cmd_list);
    exit(0);
    initialize_path(cmd_list);
    fd_c = cmd_lenght(cmd_list);
    start_execution(cmd_list, fd_c, fd_c);
}