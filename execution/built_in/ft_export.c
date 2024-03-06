/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 14:46:52 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/06 00:38:37 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

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

char **load_key_value(char *key, char *value)
{
    char **key_value;

    key_value = malloc(sizeof(char *) * 3);
    if (!key_value)
    {
        free(key);
        free(value);
        exit(EXIT_FAILURE);
    }
    key_value[0] = key;
    key_value[1] = value;
    key_value[2] = NULL;
    return key_value;
}

char *check_dollar_sign(char **env, char *value)
{
    int i;
    int k;
    int j;
    char *tmp;
    char *var;
    char *rest;

    i = -1;
    while (value[++i])
    {
        if (value[i] == '$')
        {
            k = i;
            tmp = ft_substr(value, 0, i);
            while (value[k] && value[k] != ' ' && value[k] != '=')
                k++;
            if (value[k] == '=' && value[k + 1] == '\0')
                rest = ft_strdup("=");
            else
                rest = ft_strdup(value + k);
            var = ft_substr(value, i + 1, k - i - 1);
            j = -1;
            while (env[++j])
            {
                if (ft_strncmp(env[j], var, ft_strlen(var)) == 0)
                {
                    value = ft_strjoin(tmp, ft_getenv(var, env));
                    value = ft_strjoin(value, rest);
                    i = -1;
                    break;
                }
                else
                    value = ft_strjoin(tmp, rest);
            }
        }
    }
    return value;
}
char **key_value(char *exported, char **env)
{
    char *value;
    char *key;
    char **key_value;
    int i;
    int k;
    i = 0;

    while (exported[i] && exported[i] != '=')
        i++;
    key = ft_substr(exported, 0, i);
    if (!key)
        exit(EXIT_FAILURE);
    k = i;
    while (exported[k])
        k++;
    value = ft_substr(exported, i + 1, k);
    if (ft_strchr(value, '$'))
        value = check_dollar_sign(env, value);
    if (!value)
    {
        free(key);
        exit(EXIT_FAILURE);
    }
    key_value = load_key_value(key, value);
    return key_value;
}
void ret_same_env(t_data *data, char **env)
{
    int i;
    int fd_out;

    i = 0;
    fd_out = open(ft_strjoin(data->shell_path, "/export.txt"), O_WRONLY | O_CREAT | O_APPEND | O_TRUNC, 0644);
    if (fd_out == -1)
        return check_error(fd_out, "open", 0);
    while (env[i])
    {
        write(fd_out, env[i], ft_strlen(env[i]));
        write(fd_out, "\n", 1);
        i++;
    }
    close(fd_out);
}

void add_variable(int fd_out, char *exported, int *found)
{
    if (*found == 1)
        return;
    write(fd_out, exported, ft_strlen(exported));
    write(fd_out, "\n", 1);
}

void update_key_value(int fd_out, char *key, char *value, int *found)
{
    write(fd_out, key, ft_strlen(key));
    write(fd_out, "=", 1);
    write(fd_out, value, ft_strlen(value));
    write(fd_out, "\n", 1);
    *found = 1;
}

char **add_var(char **env, char *exported)
{
    int i;
    char **new_env;

    i = 0;
    new_env = malloc(sizeof(char *) * (ft_strdoublelen(env) + 2));
    if (!new_env)
        exit(EXIT_FAILURE);
    while (env[i])
    {
        new_env[i] = env[i];
        i++;
    }
    new_env[i] = exported;
    new_env[i + 1] = NULL;
    return new_env;
}

void ft_export(t_data *data, char **env)
{
    char **key_val;
    int fd_out;
    int found;
    size_t k;
    int i;
    char *exported;

    i = 0;
    int j = 0;
    while (data->cmd->arguments[++j])
    {
        if (!check_exported(data->cmd->arguments[j]))
        {
            ret_same_env(data, env);
            continue;
        }
        fd_out = open(ft_strjoin(data->shell_path, "/export.txt"), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_out == -1)
            return check_error(fd_out, "open", 0);
        key_val = key_value(data->cmd->arguments[j], env);
        if (!key_val)
            exit(EXIT_FAILURE); 
        (1 == 1) && (i = -1, found = 0);
        while (env[++i])
        {
            k = 0;
            while (env[i][k] && env[i][k] != '=')
                k++;
            if ((ft_strncmp(env[i], key_val[0], k) == 0) && (k == ft_strlen(key_val[0])))
            {
                update_key_value(fd_out, key_val[0], key_val[1], &found);
                env[i] = ft_strjoin(key_val[0], "=");
                env[i] = ft_strjoin(env[i], key_val[1]);
            }
            else
                write(fd_out, env[i], ft_strlen(env[i]));
            write(fd_out, "\n", 1);
        }
        exported = ft_strjoin(key_val[0], "=");
        exported = ft_strjoin(exported, key_val[1]);
        add_variable(fd_out, exported, &found);
        if (ft_strdoublelen(data->cmd->arguments) > 2 && !found)
            env = add_var(env, exported);
        close(fd_out);
    }
}
