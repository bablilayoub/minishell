/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 09:55:30 by abablil           #+#    #+#             */
/*   Updated: 2024/02/26 16:48:18 by abablil          ###   ########.fr       */
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
#include <errno.h>

typedef struct s_cmd t_cmd;
typedef struct s_token t_token;
typedef struct s_data t_data;
typedef struct s_arg t_arg;

struct s_data
{
	char			*prefix;
	char			**env;
	int				exit_status;
	struct s_cmd	*cmd;
	struct s_token	*token;
};

#include "parsing/parser.h"
#include "execution/execution.h"
#include "utils/utils.h"

// Colors define
#define RED		"\033[0;31m"
#define BLACK	"\033[30m"
#define GREEN	"\033[0;32m"
#define YELLOW	"\033[0;33m"
#define BLUE	"\033[0;34m"
#define RESET	"\033[0m"
#define BOLD	"\033[1m"

// Backgrounds define
#define WHITE_BG		"\033[107m"

#define PREFIX			RESET BLUE BOLD "➜  minishell : " RESET
#define PREFIX_ERROR	RESET RED BOLD "⊗  minishell : " RESET

#endif