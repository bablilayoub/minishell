NAME = minishell
HEADER = minishell.h

CFLAGS = -Wall -Wextra -Werror

LIBFT = ./libft/libft.a

PARSING = parsing.c
EXECUTION = execution.c
MAIN = main.c

PARSING_SRCS = $(addprefix parsing/, $(PARSING))
EXECUTION_SRCS = $(addprefix execution/, $(EXECUTION))

PARSING_OBJS = $(PARSING_SRCS:.c=.o)
EXECUTION_OBJS = $(EXECUTION_SRCS:.c=.o)
MAIN_OBJS = $(MAIN:.c=.o)

$(NAME): prepare_libft $(PARSING_OBJS) $(EXECUTION_OBJS) $(MAIN_OBJS)
	@cc $(CFLAGS) -lreadline $(PARSING_OBJS) $(EXECUTION_OBJS) $(MAIN_OBJS) $(LIBFT) -o $(NAME)
	@echo "minishell is ready"

prepare_libft:
	@cd libft && make
	@echo "libft is ready"

%.o: %.c $(HEADER)
	@echo "compiling $<"
	@cc $(CFLAGS) -c $< -o $@

all: $(NAME)

clean:
	@cd libft && make clean
	@rm -f $(PARSING_OBJS) $(EXECUTION_OBJS) $(MAIN_OBJS)
	@echo "minishell is clean"

fclean: clean
	@rm -f $(NAME)
	@cd libft && make fclean
	@echo "minishell is fully clean"

re: fclean all

.PHONY: clean fclean prepare_libft
