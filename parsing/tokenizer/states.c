/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   states.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 21:21:56 by abablil           #+#    #+#             */
/*   Updated: 2024/03/01 16:02:49 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	set_in_quotes(t_token_params *params, int quote)
{
	if (quote == 1)
	{
		if (params->in_quote)
			params->in_quote = 0;
		else
			params->in_quote = 1;
	}
	else
	{
		if (params->in_dquote)
			params->in_dquote = 0;
		else
			params->in_dquote = 1;
	}
}

void	set_states(t_token_params *params, int quote)
{
	if (quote == 1 && params->in_quote)
	{
		if (params->state == IN_DQUOTE)
			params->state = IN_DQUOTE;
		else if (params->state == IN_QUOTE)
			params->state = GENERAL;
		else
			params->state = IN_QUOTE;
	}
	else if (quote == 2 && params->in_dquote)
	{
		if (params->state == IN_QUOTE)
			params->state = IN_QUOTE;
		else if (params->state == IN_DQUOTE)
			params->state = GENERAL;
		else
			params->state = IN_DQUOTE;
	}
	else
		params->state = GENERAL;
}
