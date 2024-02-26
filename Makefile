# Created Ayoub Bablil && Achraf Laalalma

# NAME
NAME = minishell

# HEADERS
HEADER = minishell.h

# FLAGS
READLINE_LIB = $(shell brew --prefix readline)/lib
READLINE_INCLUDE = $(shell brew --prefix readline)/include

CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address

# LIBRARIES
LIBFT = ./utils/libft/libft.a

# GENERAL
MAIN = main.c
UTILS = utils.c
UTILS_HEADER = ./utils/utils.h

# PARSING
PARSING_HEADER = ./parsing/parsing.h
TOKENIZER = tokenizer.c tokenizer_utils.c
CONVERTER = converter.c arguments.c commands.c
PARSING = parsing.c cleaning.c $(addprefix tokenizer/, $(TOKENIZER)) $(addprefix converter/, $(CONVERTER))

# EXECUTION
EXECUTION_HEADER = ./execution/execution.h
EXECUTION = execution.c  parent_utils_1.c parent_utils_2.c init.c built_in.c

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

parsing: prepare_libft $(PARSING_OBJS) $(UTILS_OBJS) $(MAIN_OBJS)
	@cc $(CFLAGS) -L $(READLINE_LIB) -lreadline $(PARSING_OBJS) $(UTILS_OBJS) $(MAIN_OBJS) $(LIBFT) -o $(NAME)
	@echo "parsing is ready"

execution: prepare_libft $(EXECUTION_OBJS) $(UTILS_OBJS) $(MAIN_OBJS)
	@cc $(CFLAGS) -L $(READLINE_LIB) -lreadline $(EXECUTION_OBJS) $(UTILS_OBJS) $(MAIN_OBJS) $(LIBFT) -o $(NAME)
	@echo "execution is ready"

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

# GIT COMMANDS
push:
	@echo "pushing"
	@make fclean
	@git add .
	@git commit -m "$(m)"
	@git push
	@echo "pushed"
 
force:
	@echo "pushing"
	@git add .
	@git commit -m "Force push"
	@git push -f
	@echo "pushed"

pull:
	@echo "pulling"
	@git pull
	@echo "pulled"

stash:
	@echo "stashing"
	@git stash
	@git pull
	@git stash pop
	@echo "stashed"

# PHONY
.PHONY: prepare_libft clean fclean 
