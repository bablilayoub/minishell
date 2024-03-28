/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:12:50 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/28 01:12:19 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

bool	unset_alphanumeric(char *arg)
{
	int	i;

	i = -1;
	while (arg[++i])
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (false);
	}
	return (true);
}

bool	check_unset(char *arg, bool flag, t_data *data)
{
	if (ft_isdigit(arg[0]) || !unset_alphanumeric(arg))
	{
		if (flag == true)
			printf(PREFIX_ERROR "unset: not a valid identifier\n");
		data->exit_status = 1;
		return (false);
	}
	return (true);
}

void	check_flag(char *to_update, char ***check, bool flag)
{
	char	*sub;

	if (flag == false)
	{
		sub = ft_substr(to_update, 11, ft_strlen(to_update));
		check_error_null(sub, "malloc");
		*check = ft_split(sub, '=');
		free(sub);
	}
	else
		*check = ft_split(to_update, '=');
	check_error_null(*check, "malloc");
}

void	update_(t_cmd *cmd, char ***to_update, bool flag, t_data *data)
{
	int		i;
	int		j;
	int		k;
	char	**env;
	char	**tmp;

	(1) && (i = 0, k = 0);
	while (cmd->arguments[++k])
	{
		if (!check_unset(cmd->arguments[k], flag, data))
			continue ;
		env = malloc(sizeof(char *) * (ft_strdoublelen(*to_update) + 1));
		(1) && (i = -1, j = 0);
		while ((*to_update)[++i])
		{
			check_flag((*to_update)[i], &data->chk, flag);
			if (!(ft_strncmp(data->chk[0], cmd->arguments[k],
						ft_strlen(cmd->arguments[k])) == 0
					&& ft_strlen(data->chk[0]) == ft_strlen(cmd->arguments[k])))
				env[j++] = ft_strdup((*to_update)[i]);
			free_array(data->chk);
		}
		(1) && (env[j] = NULL, tmp = *to_update, *to_update = env);
		free_array(tmp);
	}
}

void	ft_unset(t_cmd *cmd, char ***env, char ***export, t_data *data)
{
	if (!cmd->arguments[1])
		return ;
	update_(cmd, env, true, data);
	update_(cmd, export, false, data);
}
