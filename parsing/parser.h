/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:02:17 by abablil           #+#    #+#             */
/*   Updated: 2024/02/16 19:07:47 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

#include "../minishell.h"

#define GENERAL 0
#define IN_QUOTE 1
#define IN_DQUOTE 2

#define WORD "WORD"
#define WHITE_SPACE ' '
#define NEW_LINE '\n'
#define QOUTE '\''
#define DOUBLE_QUOTE '\"'
#define ESCAPE '\\'
#define ENV '$'
#define PIPE_LINE '|'
#define REDIR_IN '<'
#define REDIR_OUT '>'
#define HERE_DOC
#define DREDIR_OUT
	
typedef struct s_token
{
	char			*value;
	char			*type;
	int				state;
	size_t			len;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

// Parsing
void	reader(t_data *data);

#endif