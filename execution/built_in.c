/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:47:01 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/26 00:23:58 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void ft_echo(t_cmd *cmd)
{
    t_cmd *tmp;
    int i;

    tmp = cmd;
    i = 0;
    while (tmp->arguments[++i])
        printf("%s", tmp->arguments[i]);
    printf("\n");
}

void ft_export(t_data *data, char ***envp)
{
    char **env;
    char **new_env;
    int i;
    int fd_out;

    fd_out = open("export.txt", O_WRONLY | O_CREAT | O_APPEND | O_TRUNC, 0644);
    env = *envp;
    i = 0;
    while (env[i])
        i++;
    new_env = malloc(sizeof(char *) * (i + 2));
    i = 0;
    while (env[i])
    {
        new_env[i] = ft_strdup(env[i]);
        write(fd_out, env[i], ft_strlen(env[i]));
        write(fd_out, "\n", 1);
        i++;
    }
    new_env[i] = ft_strdup(data->cmd->arguments[1]);
    write(fd_out, data->cmd->arguments[1], ft_strlen(data->cmd->arguments[1]));
    write(fd_out, "\n", 1);
    new_env[i + 1] = NULL;
    close(fd_out);
}

int ft_chdir(t_data *data, t_cmd *cmd)
{
    (void)data;
    const char *dirname;
    int fd_out;
    
    fd_out = open("cd.txt", O_WRONLY | O_CREAT | O_APPEND | O_TRUNC, 0644);

    
    dirname = cmd->arguments[1];
    if (!dirname || (dirname[0] == '~' && dirname[1] == '\0'))
    {
        if (chdir(getenv("HOME")) == 0)
        {
            write(fd_out, getenv("HOME"), ft_strlen(getenv("HOME")));
            close(fd_out);
            return 1;
        }
        perror("chdir");
        return 2;
    }
    if (access(dirname, F_OK) == -1)
    {
        printf(PREFIX_ERROR "cd: No such file or directory %s\n", dirname);
        return 5;
    }
    if (chdir(dirname) == 0)
    {
        write(fd_out, dirname, ft_strlen(dirname));
        // write(fd_out, "\n", 1);
        return 1;
    }
    perror("chdir");
    return 0;
}

void ft_pwd()
{
    printf("%s\n", getcwd(NULL, 0));
}

void ft_env(char **env)
{
    int i;

    i = 0;
    while (env[i])
    {
        printf("%s\n", env[i]);
        i++;
    }
}

void ft_exit(t_cmd *cmd)
{
    if (cmd->arguments[1] == NULL)
        exit(EXIT_SUCCESS);
    else
        exit(ft_atoi(cmd->arguments[1]));
}

void ft_unset(t_cmd *cmd, char ***envp)
{
    char **env;
    char **new_env;
    int i;
    int j;

    env = *envp;
    i = 0;
    while (env[i])
        i++;
    new_env = malloc(sizeof(char *) * i);
    i = 0;
    j = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], cmd->arguments[1], ft_strlen(cmd->arguments[1])) != 0)
        {
            new_env[j] = ft_strdup(env[i]);
            j++;
        }
        i++;
    }
    new_env[j] = NULL;
    *envp = new_env;
}
void excute_builtin(t_cmd *cmd_list, t_data *data)
{
    if (ft_strncmp(cmd_list->arguments[0], "echo", 5) == 0)
        ft_echo(cmd_list);
    else if (ft_strncmp(cmd_list->arguments[0], "cd", 3) == 0)
        ft_chdir(data, cmd_list);
    else if (ft_strncmp(cmd_list->arguments[0], "pwd", 4) == 0)
        ft_pwd();
    else if (ft_strncmp(cmd_list->arguments[0], "env", 4) == 0)
        ft_env(data->env);
    else if (ft_strncmp(cmd_list->arguments[0], "export", 7) == 0)
        ft_export(data, &data->env);
    else if (ft_strncmp(cmd_list->arguments[0], "exit", 5) == 0)
        ft_exit(cmd_list);
    else if (ft_strncmp(cmd_list->arguments[0], "unset", 6) == 0)
        ft_unset(cmd_list, &data->env);
}