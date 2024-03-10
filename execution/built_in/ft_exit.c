/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:14:21 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/10 02:50:54 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void ft_exit(t_data *data, t_cmd *cmd)
{
    (void)data;
    if (cmd->arguments[2])
    {
        printf(PREFIX_ERROR"exit: too many arguments\n");
        exit(1);
        if (cmd->next || cmd->prev)
            exit(1);
        else
            return;
    }
    if (!cmd->arguments[1])
        exit(0);
    else
    {        
        if (cmd->arguments[1][0] == '-')
            exit(256 - ft_atoi(cmd->arguments[1] + 1));
        else
            exit(ft_atoi(cmd->arguments[1]));
    }
}
