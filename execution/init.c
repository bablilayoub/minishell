/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 19:24:33 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/26 18:02:56 by abablil          ###   ########.fr       */
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
        env = ft_split(getenv("PATH"), ':');
        i = 0;
        while (env[i])
        {
            env[i] = ft_strjoin(env[i], "/");
            env[i] = ft_strjoin(env[i], head->cmd);
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
    t_arg *arg_temp;

    temp = cmd_list;
    while (temp)
    {
        temp->arguments = malloc(sizeof(char *) * (args_lenght(temp->args) + 1));
        i = 0;
        arg_temp = temp->args;
        while (temp->args)
        {

            temp->arguments[i++] = temp->args->arg;
            temp->args = temp->args->next;
        }
        temp->args = arg_temp;
        temp->arguments[i] = NULL;
        temp = temp->next;
    }
}