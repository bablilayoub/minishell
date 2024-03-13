/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 19:24:33 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/13 01:32:25 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

bool initialize_path(t_cmd *head, t_data *data)
{
    char **env;
    bool flag = false;
    int found_error = 0;
    int i;

    while (head)
    {
        if(!head->cmd)
        {
            head = head->next;
            continue;
        }
        if (head->built_in)
        {
            head = head->next;
            continue;
        }
        if (access(head->cmd, F_OK | X_OK) == 0)
        {
            head->path = head->cmd;
            head = head->next;
            continue;
        }
        env = ft_split(ft_getenv("PATH", data->env), ':');
        if (!env)
        {
            printf(PREFIX_ERROR"%s: No such file or directory\n", head->cmd);
            return (false);
        }
        i = 0;
        while (env[i])
        {
            data->temp = env[i];
            env[i] = ft_strjoin(env[i], "/");
            free(data->temp);
            data->temp = env[i];
            env[i] = ft_strjoin(env[i], head->cmd);
            free(data->temp);
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
        free_array(env);
    }
    if (found_error)
    {
        data->exit_status = 127;
        return false;
    }
    return (true);
}

void initialize_arguments(t_cmd *cmd_list)
{
    int i;
    t_cmd *temp;
    t_arg *temp_arg;

    temp = cmd_list;
    while (temp)
    {
        i = 0;
        if (!temp->args)
        {
            temp = temp->next;
            continue;
        }
        temp->arguments = malloc(sizeof(char *) * (args_lenght(temp->args) + 1));
        temp_arg = temp->args;
        while (temp_arg)
        {
            temp->arguments[i] = ft_strdup(temp_arg->arg);
            temp_arg = temp_arg->next;
            i++;
        }
        temp->arguments[i] = NULL;
        temp = temp->next;
    }
}
