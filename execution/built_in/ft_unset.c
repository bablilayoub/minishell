/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:12:50 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/14 00:46:38 by alaalalm         ###   ########.fr       */
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
    char **tmp;
    
    i = 0;
    k = 0;
    while (cmd->arguments[++k])
    {
        if (ft_strchr(cmd->arguments[k], '='))
            continue;
        while ((*to_update)[i])
            i++;
        env = malloc(sizeof(char *) * i + 1);
        if (!env)
            exit(EXIT_FAILURE);
        i = -1;
        j = 0;
        while ((*to_update)[++i])
        {
            if (flag == false)
            {
                sub = ft_substr((*to_update)[i], 11, ft_strlen((*to_update)[i]));
                check_error_null(sub, "malloc");
                check = ft_split(sub, '=');
                free(sub);
                check_error_null(check, "malloc");
            }
            else
            {
                check = ft_split((*to_update)[i], '=');
                check_error_null(check, "malloc");
            }
            if (!check)
                exit(EXIT_FAILURE);
            if (ft_strncmp(check[0], cmd->arguments[k], ft_strlen(check[0])) == 0)
            {
                free_array(check);
                continue;
            }
            else
                env[j++] = ft_strdup((*to_update)[i]);
            free_array(check);
        }
        env[j] = NULL;
        tmp = *to_update;
        *to_update = env;
        free_array(tmp);
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
    if (!cmd->next || !cmd->prev)
        return;
}