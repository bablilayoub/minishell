/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:11:39 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/09 03:15:12 by abablil          ###   ########.fr       */
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
    while (ft_strncmp(tmp->arguments[i], "-n", ft_strlen(tmp->arguments[i])) == 0 || ft_strncmp(tmp->arguments[i], " ", 1) == 0  || ft_strncmp(tmp->arguments[i], "\t", 1) == 0)
        i++;
    i++;
    while (tmp->arguments[i])
    {
        printf("%s", tmp->arguments[i]);
        i++;
    }
    // if (ft_strncmp(tmp->arguments[1], "-n", ft_strlen(tmp->arguments[1])) != 0)
    printf("\n");
}