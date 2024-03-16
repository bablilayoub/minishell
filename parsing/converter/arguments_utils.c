/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 03:00:52 by abablil           #+#    #+#             */
/*   Updated: 2024/03/16 02:37:21 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

t_arg	*new_arg(char *value)
{
	t_arg	*arg;

	arg = (t_arg *)malloc(sizeof(t_arg));
	check_error_null(arg, "malloc");
	arg->arg = value;
	arg->next = NULL;
	arg->prev = NULL;
	return (arg);
}

t_arg	*add_arg(t_arg *head, char *value, int found_quote)
{
	t_arg	*arg;
	t_arg	*tmp;

	arg = new_arg(value);
	if (found_quote)
		arg->env_var = 0;
	else
		arg->env_var = 1;
	tmp = head;
	if (!head)
		return (arg);
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = arg;
	arg->prev = tmp;
	return (head);
}

int	is_white_space_in_quote(t_token *tmp)
{
	return (ft_strncmp(tmp->type, WHITE_SPACE, 1) == 0
		&& (tmp->state != GENERAL));
}

void	handle_general_white_space(t_token **tmp, t_arg **head, t_cmd **cmd)
{
	*tmp = skip_white_spaces(*tmp);
	if (tmp && (ft_strncmp((*cmd)->cmd, "echo", 4) == 0)
		&& not_a_shell_command(*tmp))
		*head = add_arg(*head, " ", 1);
}

int	handle_tokens(t_token **tmp, t_arg **head, t_cmd **cmd, char *tmp_type)
{
	if (ft_strncmp((*tmp)->type, WORD, 4) == 0)
	{
		*head = add_arg(*head, (*tmp)->value, 1);
		return (1);
	}
	else if (ft_strncmp((*tmp)->type, ENV, 1) == 0)
	{
		*head = add_arg(*head, (*tmp)->value, ((*tmp)->state == IN_QUOTE));
		return (1);
	}
	else if (is_redirect_out_token(*tmp))
	{
		handle_redirect_out_token(tmp, cmd, tmp_type);
		return (1);
	}
	else if (is_redirect_in_token(*tmp))
	{
		handle_redirect_in_token(tmp, cmd, tmp_type);
		return (1);
	}
	return (0);
}
