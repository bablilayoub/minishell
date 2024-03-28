/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:47:01 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/28 01:03:17 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void	excute_builtin(t_cmd *cmd_list, t_data *data)
{
	if (ft_strncmp(cmd_list->arguments[0], "echo", 4) == 0)
		ft_echo(cmd_list, data);
	else if (ft_strncmp(cmd_list->arguments[0], "cd", 2) == 0)
		ft_chdir(cmd_list, data);
	else if (ft_strncmp(cmd_list->arguments[0], "pwd", 3) == 0)
		ft_pwd(data);
	else if (ft_strncmp(cmd_list->arguments[0], "env", 3) == 0)
		ft_env(data->env, data);
	else if ((ft_strncmp(cmd_list->arguments[0], "export", 6) == 0)
		&& !cmd_list->arguments[1])
		print(data->export, data->temp);
	else if (ft_strncmp(cmd_list->arguments[0], "export", 6) == 0)
		ft_export(data, &data->env);
	else if (ft_strncmp(cmd_list->arguments[0], "exit", 4) == 0)
		ft_exit(data, cmd_list);
	else if (ft_strncmp(cmd_list->arguments[0], "unset", 5) == 0)
		ft_unset(cmd_list, &data->env, &data->export, data);
}
