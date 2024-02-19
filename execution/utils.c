/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:28:50 by alaalalm          #+#    #+#             */
<<<<<<< Updated upstream
/*   Updated: 2024/02/19 16:05:52 by alaalalm         ###   ########.fr       */
=======
/*   Updated: 2024/02/19 12:07:16 by alaalalm         ###   ########.fr       */
>>>>>>> Stashed changes
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void initialize_path(t_cmd *head)
{
    char **env;
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
                break;
            }
            i++;
        }
        head = head->next;
    }
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

<<<<<<< Updated upstream
void close_fds(int fd[][2], int fd_c)
=======
void close_fds(int fd[][2], int cmd)
>>>>>>> Stashed changes
{
    int i;
    int k;

    i = -1;
<<<<<<< Updated upstream
    while (++i < fd_c)
=======
    while (++i < cmd)
>>>>>>> Stashed changes
    {
        k = -1;
        while (++k < 2)
            close(fd[i][k]);
    }
}