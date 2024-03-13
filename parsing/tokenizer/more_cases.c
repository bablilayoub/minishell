/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_cases.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 22:54:41 by abablil           #+#    #+#             */
/*   Updated: 2024/03/12 22:55:08 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	handle_env_var(t_token_params *params,
	char *value, int len, int expandable)
{
	params->i++;
	if (params->line[params->i] >= '1' && params->line[params->i] <= '9')
		(1 == 1) && (expandable = 0, params->i++);
	temp = get_word(params->line, &params->i, 1);
	if (temp)
	{
		if (expandable)
			params->value = ft_strjoin(value, temp);
		else
			params->value = ft_strdup(temp);
		free(temp);
	}
	else
	{
		if (params->state == IN_DQUOTE || params->state == IN_QUOTE
			|| params->line[params->i] == '\0')
			params->value = ft_strdup(value);
		else
			params->value = ft_strdup("");
	}
	(1 == 1) && (params->i--, len = ft_strlen(params->value))
	if (params->value[0] == '$' && ft_strlen(params->value) == 1)
		params->type = ft_strdup(WORD);
	else
		params->type = ft_strdup(ENV);
}

void	handle_double_dollar(t_token_params *params, char *value, int len)
{
	int		parent_pid;

	parent_pid = fork();
	if (parent_pid == 0)
		exit(0);
	waitpid(parent_pid, NULL, 0);
	params->value = ft_itoa(parent_pid - 1);
	params->type = ft_strdup(WORD);
	params->i++;
}
