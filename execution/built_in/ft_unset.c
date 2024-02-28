/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:12:50 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/27 15:29:05 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void ft_unset(t_cmd *cmd, char **env)
{
    char **new_env;
    char **check;
    char **key_value;
    int fd_out;
    int i;
    int j;

    fd_out = open("unset.txt", O_WRONLY | O_CREAT | O_APPEND | O_TRUNC, 0644);
    i = 0;
    while (env[i])
        i++;
    key_value = ft_split(cmd->arguments[1], '=');
    new_env = malloc(sizeof(char *) * i);
    i = -1;
    j = 0;
    while (env[++i])
    {
        check = ft_split(env[i], '=');
        if (ft_strncmp(check[0], key_value[0], ft_strlen(check[0])) == 0)
            continue;
        else
        {
            new_env[j] = ft_strdup(env[i]);
            write(fd_out, env[i], ft_strlen(env[i]));
            write(fd_out, "\n", 1);
            j++;
        }
    }
    write(fd_out, "\n", 1);
    new_env[j] = NULL;
    close(fd_out);
}