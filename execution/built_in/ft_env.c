/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:14:02 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/13 16:43:35 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void ft_env(char **env)
{
    int i;
    char **path;
    int found;
    
    if (!env)
        return;
    path = ft_split(ft_getenv("PATH", env), ':');
    if (!path)
    {
        printf(PREFIX_ERROR"env: No such file or directory\n");
        exit(EXIT_FAILURE);
    }
    found = 0;
    i = -1;
    while (path[++i])
    {
        if (access(path[i], F_OK | X_OK) == 0)
            found = 1;
    }
    free_array(path);
    if (!found)
    {
        printf(PREFIX_ERROR"env: No such file or directory\n");
        return;
    }
    i = -1;
    while (env[++i])
        printf("%s\n", env[i]);
}
