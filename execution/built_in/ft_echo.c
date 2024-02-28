/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:11:39 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/27 16:42:03 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void ft_echo(t_cmd *cmd)
{
    t_cmd *tmp;
    int i;

    if (!cmd)
        return;
    tmp = cmd;
    i = 0;
    if (tmp->arguments[1] && ft_strncmp(tmp->arguments[1], "-n", 2) == 0)
        i = 2;
    while (tmp->arguments[++i])
    {
        if (tmp->arguments[i][0] == '\\' && (tmp->arguments[i][1] == 'n'
            || tmp->arguments[i][1] == 't'))
        {
            if (tmp->arguments[i][1] == 'n')
                printf("\n");
            else
                printf("\t");
        }
        else
            printf("%s", tmp->arguments[i]);
    }
    if (ft_strncmp(tmp->arguments[1], "-n", 2) == 0)
        printf(WHITE_BG BLACK "%c" RESET, '%');
    printf("\n");
}