/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:14:21 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/27 15:28:56 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../execution.h"

void ft_exit(t_cmd *cmd)
{
    int fd_out;

    fd_out = open("exit.txt", O_WRONLY | O_CREAT | O_APPEND | O_TRUNC, 0644);
    if (cmd->arguments[1] == NULL)
    {
        write(fd_out, "exit", 4);
        close(fd_out);
        exit(EXIT_SUCCESS);
    }
    else
    {
        write(fd_out, cmd->arguments[1], ft_strlen(cmd->arguments[1]));
        close(fd_out);
        exit(atoi(cmd->arguments[1]));
    }
}