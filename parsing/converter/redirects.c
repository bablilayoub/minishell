/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 21:58:12 by abablil           #+#    #+#             */
/*   Updated: 2024/03/15 02:27:41 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

t_redirection	*new_redirect(char *type, char *file)
{
	t_redirection	*redirect;

	redirect = (t_redirection *)malloc(sizeof(t_redirection));
	redirect->type = type;
	redirect->file = file;
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

t_token	*add_file(t_cmd **cmd, t_redirection **head, t_token *token, char *type)
{
	if (ft_strncmp(type, APPEND_OUT, 2) == 0
		|| ft_strncmp(type, REDIR_OUT, 1) == 0)
		(*cmd)->has_redir_out = 1;
	else if (ft_strncmp(type, HERE_DOC, 2) == 0
		|| ft_strncmp(type, REDIR_IN, 1) == 0)
		(*cmd)->has_redir_in = 1;
	if (!token)
		*head = add_redirect(*head, new_redirect(type, NULL));
	while (token && not_a_shell_command(token)
		&& ft_strncmp(token->type, WORD, 4) != 0)
		token = token->next;
	if (!token)
		*head = add_redirect(*head, new_redirect(type, NULL));
	if (token && ft_strncmp(token->type, WORD, 4) == 0)
		*head = add_redirect(*head, new_redirect(type, token->value));
	else
		*head = add_redirect(*head, new_redirect(type, NULL));
	return (token);
}
