/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:14:38 by alaalalm          #+#    #+#             */
/*   Updated: 2024/03/22 01:18:12 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

void	ft_pwd(t_data *data)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
	{
		printf("%s\n", strerror(errno));
		free_data(data, 1);
		data->exit_status = 1;
		exit(EXIT_FAILURE);
	}
	printf("%s\n", path);
	data->exit_status = 0;
	free(path);
}
