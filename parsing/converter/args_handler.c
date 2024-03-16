/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 02:38:10 by abablil           #+#    #+#             */
/*   Updated: 2024/03/16 02:38:20 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int	should_stop_finding_args(t_token *tmp)
{
	return (ft_strncmp(tmp->type, PIPE_LINE, 1) == 0
		&& tmp->state != IN_DQUOTE && tmp->state != IN_QUOTE);
}

int	is_redirect_out_token(t_token *tmp)
{
	return ((ft_strncmp(tmp->type, REDIR_OUT, 1) == 0
			|| ft_strncmp(tmp->type, APPEND_OUT, 2) == 0)
		&& (tmp->state != IN_QUOTE && tmp->state != IN_DQUOTE));
}

int	is_redirect_in_token(t_token *tmp)
{
	return ((ft_strncmp(tmp->type, REDIR_IN, 1) == 0
			|| ft_strncmp(tmp->type, HERE_DOC, 2) == 0)
		&& (tmp->state != IN_QUOTE && tmp->state != IN_DQUOTE));
}

void	handle_redirect_out_token(t_token **tmp, t_cmd **cmd, char *tmp_type)
{
	tmp_type = (*tmp)->type;
	if ((*tmp)->next)
		*tmp = skip_white_spaces((*tmp)->next);
	if (tmp && ft_strncmp((*tmp)->type, WORD, 4) == 0)
	{
		if (ft_strncmp(tmp_type, APPEND_OUT, 2) == 0
			|| ft_strncmp(tmp_type, REDIR_OUT, 1) == 0)
			*tmp = add_file(cmd, &(*cmd)->redirect_out, *tmp, tmp_type);
	}
	else
		*tmp = add_file(cmd, &(*cmd)->redirect_out, NULL, tmp_type);
}

void	handle_redirect_in_token(t_token **tmp, t_cmd **cmd, char *tmp_type)
{
	tmp_type = (*tmp)->type;
	if ((*tmp)->next)
		*tmp = skip_white_spaces((*tmp)->next);
	if (*tmp && ft_strncmp((*tmp)->type, WORD, 4) == 0)
	{
		if (ft_strncmp(tmp_type, HERE_DOC, 2) == 0
			|| ft_strncmp(tmp_type, REDIR_IN, 1) == 0)
			*tmp = add_file(cmd, &(*cmd)->redirect_in, *tmp, tmp_type);
	}
	else
		*tmp = add_file(cmd, &(*cmd)->redirect_in, NULL, tmp_type);
}
