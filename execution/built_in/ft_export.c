/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 14:46:52 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/09 21:00:39 by alaalalm         ###   ########.fr       */
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

void export_env_to_exp(char ***export, char **args)
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
        key_arg = ft_split(args[i], '=');
        while ((*export)[++j])
        {
            sub = ft_substr((*export)[j], 11, ft_strlen((*export)[j]));
            key = ft_split(sub, '=');
            if (ft_strncmp(key_arg[0], key[0], ft_strlen(key[0])) == 0)
            {
                if (ft_strchr(args[i], '='))
                    (*export)[j] = ft_strjoin("declare -x ", args[i]);
                found = 1;
            }
            else
                (*export)[j] = (*export)[j];
        }
        if (!found)
            *export = add_var(*export, ft_strjoin("declare -x ", args[i]));
    }
}

int check_exported(char *exported)
{
    int i;
    char **key_val;
    char *key;

    i = 0;
    if (!ft_strchr(exported, '='))
        return false;
    else if (exported[i] == '=' || (exported[i] == '$' && exported[i + 1] == '\0'))
        return (printf(PREFIX_ERROR "export: not an identifier\n"), false);
    key_val = key_value(exported);
    key = key_val[0];
    if (ft_isdigit(key[i]) || (key[i] == '_' && !key[i + 1]) || !ft_is_alphanumeric(key))
    {
        if ((key[i] == '_' && !key[i + 1]))
            return false;
        else
            return (printf(PREFIX_ERROR "export: not an identifier\n"), false);
    }
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
    if (!key)
        exit(EXIT_FAILURE);
    k = i;
    while (exported[k])
        k++;
    value = ft_substr(exported, i + 1, k);
    if (!value)
    {
        free(key);
        exit(EXIT_FAILURE);
    }
    key_value = load_key_value(key, value);
    return key_value;
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

void ft_export(t_data *data, char ***env)
{
    char **key_val;
    int found;
    size_t k;
    int i;

    i = 0;
    int j = 0;
    while (data->cmd->arguments[++j])
    {
        if (!check_exported(data->cmd->arguments[j]))
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
                (*env)[i] = ft_strjoin(key_val[0], "=");
                (*env)[i] = ft_strjoin((*env)[i], key_val[1]);
                found = 1;
            }
            else
                (*env)[i] = (*env)[i];
        }
        if (!found)
            *env = add_var(*env, data->cmd->arguments[j]);
    }
    export_env_to_exp(&data->export, data->cmd->arguments);
}
