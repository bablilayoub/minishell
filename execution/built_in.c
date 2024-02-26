/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:47:01 by alaalalm          #+#    #+#             */
/*   Updated: 2024/02/26 20:56:27 by alaalalm         ###   ########.fr       */
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

int check_exported(char *exported)
{
    int i;

    i = 0;
    if (!ft_strchr(exported, '='))
        return false;
    else if ((exported[i] == '=' && exported[i + 1] == '\0'))
        return (printf(PREFIX_ERROR "bad assignment\n"), false);
    else if (exported[i] == '=' && ft_isascii(exported[i + 1]))
        return (printf(PREFIX_ERROR "not found\n"), false);
    else if (ft_isdigit(exported[i]))
        return (printf(PREFIX_ERROR "export: not an identifier\n"), false);
    return true;
}

char **key_value(char *exported)
{
    char **split;
    char *value;
    char *key;
    char **key_value;
    int i;

    key_value = malloc(sizeof(char *) * (3));
    split = ft_split(exported, '=');
    if (!split)
        return false;
    (1 == 1) && (i = 0, key = split[i++]);
    if (exported[ft_strlen(exported) - 1] != '=')
        value = split[i++];
    else
        value = "";
    while (split[i])
    {
        value = ft_strjoin(value, "=");
        value = ft_strjoin(value, split[i]);
        i++;
    }
    i = 0;
    (1 == 1) && (key_value[i++] = key, key_value[i++] = value, key_value[i] = NULL);
    return key_value;
}
void ret_same_env(char **env)
{
    int i;
    int fd_out;

    i = 0;
    fd_out = open("export.txt", O_WRONLY | O_CREAT | O_APPEND | O_TRUNC, 0644);
    while (env[i])
    {
        write(fd_out, env[i], ft_strlen(env[i]));
        write(fd_out, "\n", 1);
        i++;
    }
    close(fd_out);
}
void ft_export(t_data *data, char **env)
{
    char **new_env;
    char **key_val;
    int i;
    int fd_out;
    char **check;
    int found = 0;

    if (!check_exported(data->cmd->arguments[1]))
    {
        ret_same_env(data->env);
        exit(EXIT_FAILURE);
    }
    key_val = key_value(data->cmd->arguments[1]);
    fd_out = open("export.txt", O_WRONLY | O_CREAT | O_APPEND | O_TRUNC, 0644);
    i = 0;
    while (env[i])
        i++;
    new_env = malloc(sizeof(char *) * (i + 2));
    i = -1;
    while (env[++i])
    {
        check = ft_split(env[i], '=');
        if (ft_strncmp(check[0], key_val[0], ft_strlen(check[0])) == 0)
        {
            new_env[i] = ft_strjoin(check[0], "=");
            new_env[i] = ft_strjoin(new_env[i], key_val[1]);
            found = 1;
        }
        else
            new_env[i] = ft_strdup(env[i]);
        write(fd_out, new_env[i], ft_strlen(new_env[i]));
        write(fd_out, "\n", 1);
    }
    if (!found)
    {
        new_env[i] = ft_strdup(data->cmd->arguments[1]);
        write(fd_out, data->cmd->arguments[1], ft_strlen(data->cmd->arguments[1]));
        write(fd_out, "\n", 1);
        new_env[i + 1] = NULL;
    }
    else
        new_env[i] = NULL;
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
            exit(EXIT_SUCCESS);
        }
        perror("chdir");
        exit(EXIT_FAILURE);
    }
    if (access(dirname, F_OK) == -1)
    {
        printf(PREFIX_ERROR "cd: No such file or directory %s\n", dirname);
        exit(EXIT_FAILURE);
    }
    if (chdir(dirname) == 0)
    {
        write(fd_out, dirname, ft_strlen(dirname));
        exit(EXIT_SUCCESS);
    }
    perror("chdir");
    exit(EXIT_FAILURE);
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
    int fd_out;

    fd_out = open("exit.txt", O_WRONLY | O_CREAT | O_APPEND | O_TRUNC, 0644);
    if (cmd->arguments[1] == NULL)
    {
        write(fd_out, "exit", 4);
        close(fd_out);
        exit(EXIT_SUCCESS);
    }
    else
    {
        write(fd_out, cmd->arguments[1], ft_strlen(cmd->arguments[1]));
        close(fd_out);
        exit(atoi(cmd->arguments[1]));
    }
}

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
        ft_export(data, data->env);
    else if (ft_strncmp(cmd_list->arguments[0], "exit", 5) == 0)
        ft_exit(cmd_list);
    else if (ft_strncmp(cmd_list->arguments[0], "unset", 6) == 0)
        ft_unset(cmd_list, data->env);
}