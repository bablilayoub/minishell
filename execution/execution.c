/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:33:23 by alaalalm          #+#    #+#             */
<<<<<<< Updated upstream
/*   Updated: 2024/02/19 16:17:26 by alaalalm         ###   ########.fr       */
=======
/*   Updated: 2024/02/19 12:07:05 by alaalalm         ###   ########.fr       */
>>>>>>> Stashed changes
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

<<<<<<< Updated upstream
void excute_childs(t_cmd *cmd, int fd[][2], int k, int fd_c)
{
    dup2(fd[k][0], STDIN_FILENO);
    dup2(fd[k + 1][1], STDOUT_FILENO);        
    close_fds(fd, fd_c);
=======
void excute_childs(t_cmd *cmd, int fd[][2], int k, int cmd_count)
{
    dup2(fd[k][0], STDIN_FILENO);
    dup2(fd[k + 1][1], STDOUT_FILENO);
    close_fds(fd, cmd_count);
>>>>>>> Stashed changes
    execve(cmd->path, cmd->arguments, NULL);
    perror("execv");
    exit(EXIT_FAILURE);
}
void start_execution(t_cmd *cmd, int fd_c, int cmd_count)
{
    int fd[fd_c][2];
    pid_t pid[cmd_count];
<<<<<<< Updated upstream
    t_cmd *current = cmd;
=======
    t_cmd *current_cmd = cmd;
>>>>>>> Stashed changes
    int k;
    int i;

    k = 0;
    i = -1;
<<<<<<< Updated upstream
    while (++i < fd_c)
        pipe(fd[i]);
    while (current)
=======

    while (++i < fd_c)
        pipe(fd[i]);

    while (current_cmd != NULL)
>>>>>>> Stashed changes
    {
        pid[k] = fork();
        if (pid[k] == -1)
            perror("fork");
        else if (pid[k] == 0)
<<<<<<< Updated upstream
            excute_childs(current, fd, k, fd_c);
        k++;
        current = current->next;
=======
        {
            excute_childs(current_cmd, fd, k, cmd_count);
            exit(EXIT_FAILURE);
        }
        k++;
        current_cmd = current_cmd->next;
>>>>>>> Stashed changes
    }
    i = -1;
    while (++i < fd_c)
        close(fd[i][1]);

<<<<<<< Updated upstream
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
=======
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
>>>>>>> Stashed changes
    }
    close_fds(fd, fd_c);
}

<<<<<<< Updated upstream
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
=======
void prepare_for_excution()
{
    int fd_c;
    t_cmd *cmd_list;

    cmd_list = parsed_list();
    fd_c = cmd_lenght(cmd_list) - 1;
    start_execution(cmd_list, fd_c, fd_c + 1);
>>>>>>> Stashed changes
}