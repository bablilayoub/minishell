/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 14:46:52 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/14 00:59:00 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

int ft_is_alphanumeric(char *key)
{
    int i;

    i = -1;
    while (key[++i])
    {
        if (!ft_isalnum(key[i]) && key[i] != '_')
            return false;
    }
    return true;
}

void export_env_to_exp(char ***export, char **args, t_data *data)
{
    int i;
    int j;
    char *sub;
    char **key;
    int found;
    char **key_arg;

    i = 0;
    while (args[++i])
    {
        j = -1;
        found = 0;
        if (!check_exported(args[i], 0))
            continue;
        key_arg = ft_split(args[i], '=');
        if (!key_arg)
            exit(EXIT_FAILURE);
        while ((*export)[++j])
        {
            sub = ft_substr((*export)[j], 11, ft_strlen((*export)[j]));
            check_error_null(sub, "malloc");
            key = ft_split(sub, '=');
            if (!key)
                exit(EXIT_FAILURE);
            if (ft_strncmp(key_arg[0], key[0], ft_strlen(key[0])) == 0)
            {
                if (ft_strchr(args[i], '='))
                {
                    data->temp = (*export)[j];
                    (*export)[j] = ft_strjoin("declare -x ", args[i]);
                    check_error_null((*export)[j], "malloc");
                    free(data->temp);
                }
                found = 1;
            }
            free_array(key);
            free(sub);
        }
        if (!found)
            add_var(export, ft_strjoin("declare -x ", args[i]), 1);
        free_double(key_arg);
    }
}

int check_exported(char *exported, int flag)
{
    int i;
    char **key_val;
    char *key;

    i = 0;
    if (!ft_strchr(exported, '=') && flag == 1)
        return false;
    else if (exported[i] == '=' || (exported[i] == '$' && exported[i + 1] == '\0'))
    {
        if (flag == 1)
            printf(PREFIX_ERROR "export: not an identifier\n");
        return false;     
    }
    key_val = key_value(exported);
    if (!key_val)
        exit(EXIT_FAILURE);
    key = key_val[0];
    if (ft_isdigit(key[i]) || (key[i] == '_' && !key[i + 1]) || !ft_is_alphanumeric(key))
    {
        if ((key[i] == '_' && !key[i + 1]))
            return false;
        else
        {
            if (flag == 1)
                printf(PREFIX_ERROR "export: not an identifier\n");
            return false;
        }
    }
    free_double(key_val);
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

char **key_value(char *exported)
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
    check_error_null(key, "malloc");
    k = i;
    while (exported[k])
        k++;
    value = ft_substr(exported, i + 1, k);
    check_error_null(value, "malloc");
    key_value = load_key_value(key, value);
    return key_value;
}

void add_var(char ***env, char *exported, int flag)
{
    int i;
    char **new_env;
    char **temp;

    i = 0;
    new_env = malloc(sizeof(char *) * (ft_strdoublelen(*env) + 2));
    if (!new_env)
        exit(EXIT_FAILURE);
    while ((*env)[i])
    {
        new_env[i] = (*env)[i];
        i++;
    }
    if (!flag)
    {
        new_env[i] = ft_strdup(exported);
        check_error_null(new_env[i], "malloc");
    }
    else
         new_env[i] = exported;
    new_env[i + 1] = NULL;
    temp = *env;
    *env = new_env;
    free(temp);
}

void ft_export(t_data *data, char ***env)
{
    char **key_val;
    int found;
    size_t k;
    int i;

    i = 0;
    int j = 0;
    key_val = NULL;
    while (data->cmd->arguments[++j])
    {
        if (!check_exported(data->cmd->arguments[j], 1))
            continue;
        key_val = key_value(data->cmd->arguments[j]);
        if (!key_val)
            exit(EXIT_FAILURE);
        (1 == 1) && (i = -1, found = 0);
        while ((*env)[++i])
        {
            k = 0;
            while ((*env)[i][k] && (*env)[i][k] != '=')
                k++;
            if ((ft_strncmp((*env)[i], key_val[0], k) == 0) && (k == ft_strlen(key_val[0])))
            {
                data->temp = (*env)[i];
                (*env)[i] = ft_strjoin(key_val[0], "=");
                check_error_null((*env)[i], "malloc");
                free(data->temp);
                data->temp = (*env)[i];
                (*env)[i] = ft_strjoin((*env)[i], key_val[1]);
                check_error_null((*env)[i], "malloc");
                free(data->temp);
                found = 1;
            }
        }
        if (!found)
            add_var(env, data->cmd->arguments[j], 0);
        free_double(key_val);
    }
    export_env_to_exp(&data->export, data->cmd->arguments, data);
    if (!data->cmd->next || !data->cmd->prev)
        return;
}
