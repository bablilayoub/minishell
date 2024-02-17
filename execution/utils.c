/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:28:50 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/17 19:20:29 by alaalalm         ###   ########.fr       */
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