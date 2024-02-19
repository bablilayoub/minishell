/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 19:24:33 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/19 21:20:16 by abablil          ###   ########.fr       */
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
        // if (((ft_strncmp(head->cmd, "clear", 5) == 0) && !head->prev)
        //     || ((ft_strncmp(head->cmd, "clear", 5) == 0) && !head->next))
        //       printf(CLEAR);
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
             printf(PREFIX_ERROR": command not found: %s\n", head->cmd);
             found_error = 1;
        }
        head = head->next;
    }
    if (found_error)
        return false;
    return (true);
}


void initialize_arguments(t_cmd *cmd_list)
{
    int i;
    t_cmd *temp;

    temp = cmd_list;
    while (cmd_list != NULL)
    {
        cmd_list->arguments = malloc(sizeof(char *) * (args_lenght(cmd_list->args) + 1));
        i = 0;
        while (cmd_list->args)
        {

            cmd_list->arguments[i++] = cmd_list->args->arg;
            cmd_list->args = cmd_list->args->next;
        }
        cmd_list->arguments[i] = NULL;
        cmd_list = cmd_list->next;
    }
    cmd_list = temp;
}