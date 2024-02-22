/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:28:50 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/22 12:53:18 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int args_lenght(t_arg *args)
{
    t_arg *tmp = args;
    
    int i = 0;
    while (tmp != NULL)
    {
        i++;
        tmp = tmp->next;
    }
    return (i);
}

int cmd_lenght(t_cmd *cmd)
{
    t_cmd *tmp = cmd;
    
    int i = 0;
    while (tmp != NULL)
    {
        i++;
        tmp = tmp->next;
    }
    return (i);
}

void close_fds(int fd[][2], int fd_c)
{
    int i;
    int k;

    i = -1;
    while (++i < fd_c)
    {
        k = -1;
        while (++k < 2)
        {
            if (write(fd[i][k], "", 0) == -1)
                continue;
            if (close(fd[i][k]) == -1)
                perror("close");
        }
    }
}
