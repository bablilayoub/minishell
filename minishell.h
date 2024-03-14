/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaalalm <alaalalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 09:55:30 by abablil           #+#    #+#             */
/*   Updated: 2024/03/12 03:02:30 by alaalalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "utils/libft/libft.h"

typedef struct s_data
{
	char			*prefix;
	char			**env;
	char			*shell_path;
	char			**export;
	char			**in_valid;
	int				exit_status;
	char			*temp;
	char 			**_2Dtmp;
	struct s_cmd	*cmd;
	struct s_token	*token;
}	t_data;

typedef struct s_token
{
	char			*value;
	char			*type;
	int				state;
	size_t			len;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_token_params
{
	size_t	i;
	size_t	len;
	char	*line;
	char	*value;
	char	*type;
	int		state;
	int		in_quote;
	int		in_dquote;
	t_token	*head;
	t_token	*token;
}	t_token_params;

typedef struct s_arg
{
	char			*arg;
	int				env_var;
	struct s_arg	*next;
	struct s_arg	*prev;
}	t_arg;

typedef struct s_redirection
{
	char					*type;
	char					*file;
	struct s_redirection	*next;
	struct s_redirection	*prev;
}	t_redirection;

typedef struct s_cmd
{
	char					*cmd;
	char					*path;
	bool					has_pipe;
	char					**arguments;
	bool					built_in;
	char					*pwd;
	char					*old_pwd;
	int						found;
	int						has_redir_in;
	int						has_redir_out;
	struct s_redirection	*redirect_in;
	struct s_redirection	*redirect_out;
	struct s_arg			*args;
	struct s_cmd			*next;
	struct s_cmd			*prev;
}	t_cmd;

// Colors
# define RED	"\033[0;31m"
# define BLACK	"\033[30m"
# define GREEN	"\033[0;32m"
# define YELLOW	"\033[0;33m"
# define BLUE	"\033[0;34m"
# define RESET	"\033[0m"
# define BOLD	"\033[1m"

// Backgrounds
# define WHITE_BG		"\033[107m"

// Prefix
# define PREFIX			"\033[0m\033[0;34m\033[1m➜  minishell : \033[0m"
# define PREFIX_ERROR	"\033[0\033[0;31m\033[1m⊗  minishell : \033[0m"

#endif