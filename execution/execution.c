/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:33:23 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/17 19:20:01 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void start_execution(t_cmd *cmd)
{
    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if (pid == 0)
    {
        close(fd[0]);
        // dup2(fd[1], 1);
        // close(fd[1]);
        execve(cmd->path, cmd->arguments, NULL);
    }
    // else
    // {
    //     close(fd[1]);
    //     dup2(fd[0], 0);
    //     close(fd[0]);
    //     execve(cmd->path, cmd->args, NULL);
    // }
}


void executer(t_data *data)
{
    t_cmd *list_cmd;
    int   argc;
    char **argv;
    int i;
    
    list_cmd = data->cmd;
    argc = args_lenght(list_cmd->args);
    argv = malloc(sizeof(char *) * (argc + 1));
    initialize_path(list_cmd);
    i = 0;
    while (list_cmd->args != NULL)
    {
        argv[i] = list_cmd->args->arg;
        printf("%s\n", list_cmd->cmd);
        printf("%s\n", argv[i]);
        i++;
        list_cmd->args = list_cmd->args->next;
    }
    argv[i] = NULL;
    list_cmd->arguments = argv;
    // print_commands(list_cmd);
    // start_execution(list_cmd);
}