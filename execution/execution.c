/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:33:23 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/17 18:45:40 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// void execution(t_cmd *cmd)
// {
//     int fd[2];
//     pid_t pid;

//     printf("path: %s\n", cmd->args[0]);
//     printf("path: %s\n", cmd->args[1]);
//     exit(0);
//     if (pipe(fd) == -1)
//     {
//         perror("pipe");
//         exit(1);
//     }

//     pid = fork();
//     if (pid == 0)
//     {
//         close(fd[0]);
//         dup2(fd[1], 1);
//         close(fd[1]);
//         execve(cmd->path, cmd->args, NULL);
//     }
//     // else
//     // {
//     //     close(fd[1]);
//     //     dup2(fd[0], 0);
//     //     close(fd[0]);
//     //     execve(cmd->path, cmd->args, NULL);
//     // }
// }

void executer(t_data *data)
{
    t_cmd *list_cmd;
    
    list_cmd = data->cmd;
    
    initialize_path(list_cmd);
    while (list_cmd != NULL)
    {
        printf("path: %s\n", list_cmd->path);
        while (list_cmd->args)
        {
            printf("arg : %s | env_var : %d\n", list_cmd->args->arg, list_cmd->args->env_var);
            list_cmd->args = list_cmd->args->next;
        }
        list_cmd = list_cmd->next;
    }
    // print_commands(list_cmd);
    // execution(list_cmd);
}