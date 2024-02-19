/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:28:50 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/19 20:39:20 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

bool initialize_path(t_cmd *head)
{
    char **env;
    bool flag = false;
    int found_error = 0;
    int i;

    while (head)
    {
        env = ft_split(getenv("PATH"), ':');
        i = 0;
        while (env[i])
        {
            env[i] = ft_strjoin(env[i], "/");
            env[i] = ft_strjoin(env[i], head->cmd);
            i++;
        }
        i = 0;
        while (env[i])
        {
            if (access(env[i], F_OK | X_OK) == 0)
            {
                head->path = env[i];
                flag = true;
                break;
            }
            i++;
        }
        if (!flag)
        {
             printf(PREFIX_ERROR"command not found: %s\n", head->cmd);
             found_error = 1;
        }
        head = head->next;
    }
    if (found_error)
        return false;
    return (true);
}

int args_lenght(t_arg *args)
{
    int i = 0;
    while (args != NULL)
    {
        i++;
        args = args->next;
    }
    return (i);
}

int cmd_lenght(t_cmd *cmd)
{
    int i = 0;
    while (cmd != NULL)
    {
        i++;
        cmd = cmd->next;
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
            close(fd[i][k]);
    }
}