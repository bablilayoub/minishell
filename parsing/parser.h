/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:02:17 by abablil           #+#    #+#             */
/*   Updated: 2024/02/17 19:13:55 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

#include "../minishell.h"

#define GENERAL 0
#define IN_QUOTE 1
#define IN_DQUOTE 2

#define WORD "WORD"
#define WHITE_SPACE " "
#define NEW_LINE "\n"
#define QUOTE "\'"
#define DOUBLE_QUOTE "\""
#define ESCAPE "\\"
#define ENV "$"
#define PIPE_LINE "|"
#define REDIR_IN "<"
#define REDIR_OUT ">"
#define HERE_DOC "<<"
#define APPEND_OUT ">>"
	
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
	char 	*line;
	char 	*value;
	char 	*type;
	int 	state;
	int 	in_quote;
	int		in_dquote;
	t_token	*head;
	t_token	*token;
}	t_token_params;

struct s_arg
{
	char			*arg;
	int				env_var;
	struct s_arg	*next;
	struct s_arg	*prev;
};

struct s_cmd
{
	char			*cmd;
	char			*path;
	char			*redirect;
	char			*output_file;
	struct s_arg	*args;
	struct s_cmd	*next;
	struct s_cmd	*prev;
};

// Parsing
void	reader(t_data *data);

// Tokenizer
void	*tokenizer(char *line);

// Tokenizer utils
t_token	*new_token(char *value, char *type, int state, size_t len);
t_token	*add_token(t_token *head, t_token *token);
char	*get_word(char *line, size_t *i);
void	print_tokens(t_token *token);

// Converter
void	convert_tokens_to_commands(t_data *data);
int		not_a_shell_command(t_token *token);

// Converter utils
t_cmd	*new_cmd(t_token *token);
t_cmd	*add_cmd(t_cmd *head, t_cmd *cmd);

// Arguments
t_arg	*new_arg(char *value);
t_arg	*add_arg(t_arg *head, char *value, int found_quote);
void	find_args(t_cmd *cmd, t_token *token);

#endif