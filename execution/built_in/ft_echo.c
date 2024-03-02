/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:11:39 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/02 20:05:26 by abablil          ###   ########.fr       */
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
    while (ft_strncmp(tmp->arguments[i], "-n", 2) == 0 
        || ft_strncmp(tmp->arguments[i], " ", 1) == 0 
        || ft_strncmp(tmp->arguments[i], "\t", 1) == 0)
        i++;
    while (tmp->arguments[i])
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
        i++;
    }
    if (ft_strncmp(tmp->arguments[1], "-n", 2) == 0)
        printf(WHITE_BG BLACK "%c" RESET, '%');
    printf("\n");
}