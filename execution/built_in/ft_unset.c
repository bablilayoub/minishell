/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:12:50 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/09 21:11:19 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void update_(t_cmd *cmd, char ***to_update, bool flag, char **check)
{
    int i;
    int j;
    int k;
    char **env;
    char *sub;

    i = 0;
    k = 0;
    while (cmd->arguments[++k])
    {
        if (ft_strchr(cmd->arguments[k], '='))
            continue;
        while ((*to_update)[i])
            i++;
        env = malloc(sizeof(char *) * i);
        i = -1;
        j = 0;
        while ((*to_update)[++i])
        {
            if (flag == false)
            {
                sub = ft_substr((*to_update)[i], 11, ft_strlen((*to_update)[i]));
                check = ft_split(sub, '=');
            }
            else
                check = ft_split((*to_update)[i], '=');
            if (ft_strncmp(check[0], cmd->arguments[k], ft_strlen(check[0])) == 0)
                continue;
            else
                env[j++] = ft_strdup((*to_update)[i]);
        }
        env[j] = NULL;
        *to_update = env;
    }
}

void ft_unset(t_cmd *cmd, char ***env, char ***export)
{
    char **check;

    if (!cmd->arguments[1])
        return;
    check = NULL;
    update_(cmd, env, true, check);
    update_(cmd, export, false, check);
}