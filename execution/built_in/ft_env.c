/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:14:02 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/10 23:52:16 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void ft_env(char **env)
{
    int i;

    if (!env)
        return;
    char **path = ft_split(ft_getenv("PATH", env), ':');
    if (!path)
    {
        printf(PREFIX_ERROR"env: No such file or directory\n");
        return;
    }
    int found = 0;
    i = -1;
    while (path[++i])
    {
        if (access(path[i], F_OK | X_OK) == 0)
            found = 1;
    }
    if (!found)
    {
        printf(PREFIX_ERROR"env: No such file or directory\n");
        return;
    }
    i = -1;
    while (env[++i])
        printf("%s\n", env[i]);
}