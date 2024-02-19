/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 09:55:30 by abablil           #+#    #+#             */
/*   Updated: 2024/02/19 18:10:25 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "utils/libft/libft.h"

typedef struct s_cmd t_cmd;
typedef struct s_token t_token;
typedef struct s_data t_data;
typedef struct s_arg t_arg;

struct s_data
{
	char		**env;
	struct s_cmd	*cmd;
	struct s_token	*token;
};

#include "parsing/parser.h"
#include "execution/execution.h"
#include "utils/utils.h"

#define PREFIX			"minishell$ "
#define PREFIX_ERROR	"msh"

#endif