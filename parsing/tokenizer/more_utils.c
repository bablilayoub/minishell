/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 00:19:19 by abablil           #+#    #+#             */
/*   Updated: 2024/03/21 00:19:43 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	when_no_word(t_token_params *params, char *value)
{
	if ((params->line[params->i] == '"' || params->line[params->i] == '\'')
		&& params->state == GENERAL)
		params->value = ft_strdup("");
	else
		params->value = ft_strdup(value);
}

void	finalize_token(t_token_params *params, int len, char *value)
{
	params->token = new_token(params->value, params->type, params->state, len);
	params->head = add_token(params->head, params->token);
	if (len > 1 && value[0] != '\\')
		params->i++;
}
