/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 21:58:12 by abablil           #+#    #+#             */
/*   Updated: 2024/04/11 18:39:03 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

t_redirection	*new_redirect(char *type, char *file, int expandable)
{
	t_redirection	*redirect;

	redirect = (t_redirection *)malloc(sizeof(t_redirection));
	check_error_null(redirect, "malloc");
	redirect->type = type;
	redirect->file = file;
	redirect->expandalbe = expandable;
	redirect->prev = NULL;
	redirect->next = NULL;
	return (redirect);
}

t_redirection	*add_redirect(t_redirection *head, t_redirection *redirect)
{
	t_redirection	*tmp;

	tmp = head;
	if (!head)
		return (redirect);
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = redirect;
	redirect->prev = tmp;
	return (head);
}

int	file_not_found(t_token *token)
{
	return (token && not_a_shell_command(token)
		&& ft_strncmp(token->type, WORD, 4) != 0
		&& ft_strncmp(token->type, ENV, 1) != 0
		&& ft_strncmp(token->type, SPECIAL_CASE, 12) != 0);
}

t_token	*add_file(t_cmd **cmd, t_redirection **head, t_token *token, char *type)
{
	(*cmd)->has_redirection = 1;
	if (!token)
		*head = add_redirect(*head, new_redirect(type, NULL, 0));
	while (file_not_found(token))
		token = token->next;
	if (!token)
		*head = add_redirect(*head, new_redirect(type, NULL, 0));
	if (token && (ft_strncmp(token->type, WORD, 4) == 0
			|| ft_strncmp(token->type, SPECIAL_CASE, 12) == 0
			|| ft_strncmp(token->type, ENV, 1) == 0))
	{
		if (ft_strncmp(type, HERE_DOC, 2) == 0)
			*head = add_redirect(*head,
					new_redirect(type, ft_strdup(token->value),
						token->state == GENERAL));
		else
			*head = add_redirect(*head,
					new_redirect(type, ft_strdup(token->value),
						(token->state != IN_QUOTE)));
	}
	else
		*head = add_redirect(*head, new_redirect(type, NULL, 0));
	return (token);
}
