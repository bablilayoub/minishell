/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 09:55:30 by abablil           #+#    #+#             */
/*   Updated: 2024/02/16 12:05:06 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
#include "parsing/parser.h"
#include "execution/execution.h"

typedef struct s_history
{
	int					id;
	char				*cmd;
	struct s_history	*next;
	struct s_history	*prev;
}				t_history;

typedef struct s_data
{
	
}				t_data;

#endif