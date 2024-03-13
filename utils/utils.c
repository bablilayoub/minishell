/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:57:17 by abablil           #+#    #+#             */
/*   Updated: 2024/03/12 22:56:26 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	only_spaces(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '\t')
			count++;
		i++;
	}
	if (count == i)
		return (true);
	return (false);
}

char	**allocate_env(char **env)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
	{
		printf(PREFIX_ERROR"Error: failed to allocate memory\n");
		exit(1);
	}
	i = -1;
	while (env[++i])
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			printf(PREFIX_ERROR"Error: failed to allocate memory\n");
			exit(1);
		}
	}
	new_env[i] = NULL;
	return (new_env);
}

char	**allocate_export(char **env)
{
	int		i;
	int		lenght;
	char	**tmp;

	lenght = 0;
	while (env[lenght])
		lenght++;
	tmp = malloc(sizeof(char *) * (lenght + 1));
	if (!tmp)
		exit(EXIT_FAILURE);
	i = -1;
	while (env[++i])
		tmp[i] = ft_strjoin("declare -x ", env[i]);
	tmp[i] = NULL;
	return (tmp);
}

t_token	*skip_white_spaces(t_token *token)
{
	t_token	*tmp;

	if (!token)
		return (NULL);
	tmp = token;
	while (tmp && ft_strncmp(tmp->type, WHITE_SPACE, 1) == 0)
		tmp = tmp->next;
	return (tmp);
}
