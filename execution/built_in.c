/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:47:01 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/22 19:21:56 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void ft_echo(t_cmd *cmd)
{
    t_cmd *tmp;
    int     i;
    
    tmp = cmd;
    i = 1;
    while (tmp->arguments[i])
    {
        printf("%s", tmp->arguments[i]);
        i++;
    }
    printf("\n");
}

int ft_chdir(t_cmd *cmd)
{
    DIR *dir;
    struct dirent *entry;
    const char *dirname = cmd->arguments[1];
    
    
    if (cmd->arguments[1] == NULL)
    {
        if (chdir(getenv("HOME")) == 0)
            return 1;
    }
    dir = opendir(".");
    if (dir == NULL)
    {
        perror("opendir");
        return 2;
    }
    
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR && ft_strncmp(entry->d_name, dirname, ft_strlen(dirname)) == 0)
        {
            if (chdir(entry->d_name) == 0)
            {
                closedir(dir);
                return 3;
            }
            else
            {
                perror("chdir");
                closedir(dir);
                return 4;
            }
        }
    }
    closedir(dir);
    printf("cd: no such file or directory: %s\n", dirname);
    return 0;
}

void ft_pwd()
{
    printf("%s\n", getcwd(NULL, 0));
}

void ft_env(char **env)
{
    int i = -1;
    
    while (env[++i])
        printf("%s\n", env[i]);
}

void ft_export(t_cmd *cmd, char ***envp)
{
    char **new_env;
    char **env;
    int i;

    env = *envp;
    i = 0;
    while (env[i])
        i++;
    new_env = malloc(sizeof(char *) * (i + 2));
    i = 0;
    while (env[i])
    {
        new_env[i] = ft_strdup(env[i]);
        i++;
    }
    new_env[i] = ft_strdup(cmd->arguments[1]);
    new_env[i + 1] = NULL;
    *envp = new_env;
}

void ft_exit(t_cmd *cmd)
{
    if (cmd->arguments[1] == NULL)
        exit(EXIT_SUCCESS);
    else
        exit(ft_atoi(cmd->arguments[1]));
}

void builtin_functions(t_cmd *cmd_list, t_data *data)
{
    static char **env;

    if (!env)
        env = data->env;
    if (ft_strncmp(cmd_list->cmd, "echo", 5) == 0)
        ft_echo(cmd_list);
    else if (ft_strncmp(cmd_list->cmd, "cd", 3) == 0)
        ft_chdir(cmd_list);
    else if (ft_strncmp(cmd_list->cmd, "pwd", 4) == 0)
        ft_pwd();
    else if (ft_strncmp(cmd_list->cmd, "env", 4) == 0 && cmd_list->arguments[1] == NULL)
    {
        ft_env(env);
        return ;
    }
    else if (ft_strncmp(cmd_list->cmd, "exit", 5) == 0)
        ft_exit(cmd_list);
    else if ((ft_strncmp(cmd_list->cmd, "export", 7) == 0))
    {
        ft_export(cmd_list, &env);
        return ;
    }
}