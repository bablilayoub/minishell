/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:57:17 by abablil           #+#    #+#             */
/*   Updated: 2024/03/23 18:53:22 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../parsing/parser.h"

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
	check_error_null(new_env, "malloc");
	i = -1;
	while (env[++i])
	{
		new_env[i] = ft_strdup(env[i]);
		check_error_null(new_env[i], "malloc");
	}
	new_env[i] = NULL;
	return (new_env);
}

char	**allocate_export(char **env)
{
	int		i;
	int		j;
	int		flag;
	int		lenght;
	char	**tmp;

	lenght = ft_strdoublelen(env);
	tmp = malloc(sizeof(char *) * (lenght + 1));
	check_error_null(tmp, "malloc");
	(1) && (i = -1, j = 0, flag = 0);
	while (env[++i])
	{
		if (ft_strncmp(env[i], "OLDPWD", 6) == 0)
		{
			tmp[i] = ft_strjoin("declare -x ", "OLDPWD");
			flag = 1;
		}
		else
		{
			tmp[i] = ft_strjoin("declare -x ", env[i]);
			check_error_null(tmp[i], "malloc");
		}
	}
	tmp[i] = NULL;
	if (!flag)
		add_var(&tmp, ft_strjoin("declare -x ", "OLDPWD"), 1);
	return (tmp);
}

bool	between_dquotes(t_token *token)
{
	t_token	*tmp;
	int		count;

	count = 0;
	tmp = token;
	while (tmp && ft_strncmp(tmp->type, QUOTE, 1) != 0)
	{
		if (ft_strncmp(tmp->type, DOUBLE_QUOTE, 1) == 0)
			count++;
		tmp = tmp->prev;
	}
	return (count % 2 == 0);
}

void	get_parent_pid(t_data *data)
{
	int	ret;

	ret = fork();
	if (ret == -1)
	{
		printf(PREFIX_ERROR"Error: fork failed\n");
		exit(1);
	}
	if (ret == 0)
		exit(0);
	data->parent_pid = ret - 1;
}
