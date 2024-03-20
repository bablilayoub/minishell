/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abablil <abablil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:02:17 by abablil           #+#    #+#             */
/*   Updated: 2024/03/19 22:53:40 by abablil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../minishell.h"
# include "../utils/utils.h"
# include "../execution/execution.h"

# define GENERAL 0
# define IN_QUOTE 1
# define IN_DQUOTE 2

# define WORD "WORD"
# define WHITE_SPACE " "
# define NEW_LINE "\n"
# define TAB_SPACE "\t"
# define QUOTE "\'"
# define DOUBLE_QUOTE "\""
# define ESCAPE "\\"
# define ENV "$"
# define PIPE_LINE "|"
# define REDIR_IN "<"
# define REDIR_OUT ">"
# define HERE_DOC "<<"
# define APPEND_OUT ">>"
# define SPECIAL_CASE "SPECIAL_CASE"

// Parsing
void	reader(t_data *data);

// Tokenizer
void	*tokenizer(char *line);

// Tokenizer utils
t_token	*new_token(char *value, char *type, int state, size_t len);
t_token	*add_token(t_token *head, t_token *token);
char	*get_word(char *line, size_t *i, int alpha_num);
t_token	*skip_white_spaces(t_token *token);

// Clean tokens
t_token	*clean_tokens(t_data *data, t_token *head);

// States
void	set_in_quotes(t_token_params *params, int quote);
void	set_states(t_token_params *params, int quote);

// Cases
void	handle_special_char(t_token_params *params, char *value, int len);
void	handle_quotes(t_token_params *params, int quote, char *quote_type);
void	handle_word(t_token_params *params, char *line);
void	handle_cases(t_token_params *params, char *line);
void	handle_quotes(t_token_params *params, int quote, char *quote_type);
void	handle_more_cases(t_token_params *params, char *line);
void	handle_else(t_token_params *params, char *value);

// Converter
void	convert_tokens_to_commands(t_data *data);
int		not_a_shell_command(t_token *token);
bool	is_built_in(char *value);

// Converter utils
int		check_syntax_error(t_token *token);

// Redirects
t_token	*add_file(t_cmd **cmd, t_redirection **head,
			t_token *token, char *type);

// Commands
t_cmd	*new_cmd(t_token *token);
t_cmd	*add_cmd(t_cmd *head, t_cmd *cmd);

// Converter utils
int		not_a_shell_command(t_token *token);
t_token	*get_command_name(t_token *head);
int		cmd_starts_with_redir(t_token *tmp);

// Arguments
t_token	*find_args(t_cmd *cmd, t_token *token);

// Arguments handler
int		should_stop_finding_args(t_token *tmp);
int		is_redirect_token(t_token *tmp);
void	handle_redirect_token(t_token **tmp, t_cmd **cmd, char *tmp_type);

// Arguments utils
t_arg	*new_arg(char *value);
t_arg	*add_arg(t_arg *head, char *value, int found_quote);
int		is_white_space_in_quote(t_token *tmp);
void	handle_general_white_space(t_token **tmp, t_arg **head, t_cmd **cmd);
int		handle_tokens(t_token **tmp, t_arg **head, t_cmd **cmd, char *tmp_type);

// Cleaning
void	free_data(t_data *data, int free_all);
void	free_token(t_token *token);
void	free_tokens(t_token *token);
void	free_array(char **array);
void	free_args(t_arg *args);

// Signals
void	signal_handler(int sig);

// Syntax
int		check_quotes(t_token *token);
int		check_pipes(t_cmd *cmd);
int		check_syntax(t_data *data);

// Env
char	*get_env(char *env_var, t_data *data);
void	get_env_vars(t_data *data);

// Debug
void	print_args(t_cmd *head);
void	print_tokens(t_token *token);
#endif