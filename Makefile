# Created Ayoub Bablil && Achraf Laalalma

# NAME
NAME = minishell

# HEADERS
HEADER = minishell.h

# FLAGS
READLINE_LIB = $(shell brew --prefix readline)/lib
READLINE_INCLUDE = $(shell brew --prefix readline)/include

CFLAGS = -Wall -Wextra -Werror -g

# LIBRARIES
LIBFT = ./utils/libft/libft.a

# GENERAL
MAIN = main.c
UTILS = utils.c more_utils.c
UTILS_HEADER = ./utils/utils.h

# PARSING
PARSING_HEADER = ./parsing/parsing.h
TOKENIZER = tokenizer.c clean_tokens.c tokenizer_utils.c cases.c more_cases.c states.c more_utils.c
CONVERTER = converter.c converter_utils.c arguments.c args_handler.c arguments_utils.c commands.c env.c redirects.c
TOOLS = cleaning.c more_cleaning.c signals.c syntax.c
PARSING = parsing.c $(addprefix tokenizer/, $(TOKENIZER)) $(addprefix converter/, $(CONVERTER)) $(addprefix tools/, $(TOOLS))

# EXECUTION
EXECUTION_HEADER = ./execution/execution.h
BUILTIN = built_in.c ft_chdir.c ft_echo.c ft_env.c ft_exit.c ft_export.c export_utils1.c export_utils2.c ft_pwd.c ft_unset.c \
			here_doc.c redirections.c redirections2.c print_exp.c
EXECUTION = execution.c  utils1.c utils2.c utils3.c utils4.c init.c $(addprefix built_in/, $(BUILTIN))

# SOURCES
PARSING_SRCS = $(addprefix parsing/, $(PARSING))
EXECUTION_SRCS = $(addprefix execution/, $(EXECUTION))
UTILS_SRCS = $(addprefix utils/, $(UTILS))

# OBJECTS
PARSING_OBJS = $(PARSING_SRCS:.c=.o)
EXECUTION_OBJS = $(EXECUTION_SRCS:.c=.o)
UTILS_OBJS = $(UTILS_SRCS:.c=.o)
MAIN_OBJS = $(MAIN:.c=.o)

# HEADERS
HEADERS = $(HEADER) $(PARSING_HEADER) $(EXECUTION_HEADER) $(UTILS_HEADER)

# RULES
$(NAME): prepare_libft $(PARSING_OBJS) $(EXECUTION_OBJS) $(UTILS_OBJS) $(MAIN_OBJS)
	@cc $(CFLAGS) -L $(READLINE_LIB) -lreadline $(PARSING_OBJS) $(EXECUTION_OBJS) $(UTILS_OBJS) $(MAIN_OBJS) $(LIBFT) -o $(NAME)
	@echo "minishell is ready"

prepare_libft:
	@cd ./utils/libft && make
	@echo "libft is ready"

%.o: %.c $(HEADER)
	@echo "compiling $<"
	@cc $(CFLAGS) -I $(READLINE_INCLUDE)  -c $< -o $@

all: $(NAME)

clean:
	@cd ./utils/libft && make clean
	@rm -f $(PARSING_OBJS) $(EXECUTION_OBJS) $(MAIN_OBJS) $(UTILS_OBJS)
	@echo "minishell is clean"

fclean: clean
	@rm -f $(NAME)
	@cd ./utils/libft && make fclean
	@echo "minishell is fully clean"

re: fclean all

# PHONY
.PHONY: prepare_libft clean fclean 