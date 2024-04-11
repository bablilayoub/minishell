/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_ambiguous.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 13:12:06 by abablil           #+#    #+#             */
/*   Updated: 2024/04/11 13:16:29 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int	is_redirect(t_token *tmp)
{
	return (ft_strncmp(tmp->type, REDIR_OUT, ft_strlen(tmp->type)) == 0
		|| ft_strncmp(tmp->type, REDIR_IN, ft_strlen(tmp->type)) == 0
		|| ft_strncmp(tmp->type, APPEND_OUT, ft_strlen(tmp->type)) == 0);
}

int	is_whitespace_or_quotes(t_token *tmp)
{
	return (ft_strncmp(tmp->type, WHITE_SPACE, 1) == 0
		|| ft_strncmp(tmp->type, DOUBLE_QUOTE, 1) == 0
		|| ft_strncmp(tmp->type, QUOTE, 1) == 0);
}

int	handle_env(t_token *tmp, char **file, t_data *data, t_token *tokens)
{
	if (!tmp->value[1] || tmp->value[1] == '$')
		return (1);
	*file = get_env(tmp->value + 1, data);
	if (tmp->state == GENERAL)
	{
		if (!handle_general_state(*file, tokens))
			return (0);
	}
	else if (tmp->state == IN_DQUOTE)
	{
		if (!handle_in_dquote_state(*file, tokens))
			return (0);
	}
	else
	{
		free(*file);
		return (1);
	}
	return (1);
}

int	handle_general_state(char *file, t_token *tokens)
{
	if (!file)
	{
		printf("%s\n", PREFIX_ERROR "Ambiguous redirect");
		free(file);
		free_tokens(tokens);
		return (0);
	}
	free(file);
	return (1);
}

int	handle_in_dquote_state(char *file, t_token *tokens)
{
	if (!file)
	{
		printf("%s\n", PREFIX_ERROR "No such file or directory");
		free(file);
		free_tokens(tokens);
		return (0);
	}
	free(file);
	return (1);
}
