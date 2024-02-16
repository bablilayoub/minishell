/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 09:55:30 by abablil           #+#    #+#             */
/*   Updated: 2024/02/16 16:33:41 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "utils/libft/libft.h"
#include "parsing/parser.h"
#include "execution/execution.h"

#define PREFIX "minishell$ "

typedef struct s_cmd
{
	char	*cmd;
	char	**options;
	char	**args;
}				t_cmd;

typedef struct s_data
{
	char		**env;
	char		**args;
	t_cmd		*cmd;
}				t_data;




#endif