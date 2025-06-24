#◉────▣───▢◇▢───▣────◉•◉───▣───▢   Colores:   ▢───▣───◉•◉───▣────▢◇▢───▣───◉#

BLACK   := \033[0;30m
RED     := \033[0;31m
GREEN   := \033[0;32m
YELLOW  := \033[0;33m
BLUE    := \033[0;34m
MAGENTA := \033[0;35m
CYAN    := \033[0;36m

CURRENT_COLOR := \033[0;34m
RESET   := \033[0m

#◉───▣───▢◇▢───▣───◉•◉── Variables del proyecto ◉•◉───▣───▢◇▢───▣───◉#

NAME        := minishell
CC          := cc
CFLAGS      := -Wall -Werror -Wextra -g
RM          := rm -rf

READLINE_INC := -I/usr/local/opt/readline/include #he tenido que modificarlo porque lo tengo en otra ubicacion
READLINE_LIB := -L/usr/local/opt/readline/lib -lreadline

SRC_DIR     := src
OBJ_DIR     := obj
INC_DIR     := inc

LIBFT_DIR	:= libft/
LIBFT		:= $(LIBFT_DIR)/libft.a
INCLUDES_LIBFT := -I$(LIBFT_DIR)
INCLUDES_MINISHELL := -I$(INC_DIR)

# Archivos fuente
SRCS := \
	main.c \
	builtins/builtins.c builtins/cd.c builtins/echo.c builtins/env.c builtins/exit.c builtins/export.c builtins/export_utils.c builtins/export_utils_2.c builtins/pwd.c builtins/unset.c \
	executor/executor_single_cmd.c executor/executor.c executor/executor_loop.c executor/get_path_execve.c executor/here_doc.c executor/here_doc_utils.c executor/executor_utils.c executor/built_args.c executor/redirections.c executor/env_init.c executor/env_utils.c executor/pipes.c \
	parcer/var_check_expand.c parcer/var_expand_core.c parcer/var_lookup.c \
	parcer/check_spaces_line.c parcer/check_spaces_line_utils.c \
	parcer/syntax_error_token_utils.c parcer/syntax_error_token.c \
	parcer/tokens_type_check_type.c parcer/tokens_type_command_check.c parcer/tokens_type_redir_check.c parcer/tokens_type_redir_context_check.c parcer/tokens_type_pipe_check.c \
	parcer/ckeck_quote_utils.c parcer/free_utils.c parcer/free_structs.c parcer/ft_split_with_quotes.c parcer/init_list.c \
	parcer/init_structures.c parcer/init_structures_utils.c \
	parcer/quit_quotes_token.c parcer/tokenizer.c parcer/tokenizer_utils.c parcer/get_prompt.c parcer/handle_error.c parcer/minishell_loop.c \
	utils/signals.c utils/herequote_utils.c \
	test_utils.c

SRC_PATHS := $(addprefix $(SRC_DIR)/,$(SRCS))
OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

#◉── Reglas principales ──────────────◉

all: $(LIBFT) $(NAME)

$(LIBFT): 
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(READLINE_LIB) -L$(LIBFT_DIR) -lft

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES_MINISHELL) $(INCLUDES_LIBFT) $(READLINE_INC) -c $< -o $@

$(OBJ_DIR): 
	@mkdir -p $(OBJ_DIR) $(OBJ_DIR)/builtins $(OBJ_DIR)/executor $(OBJ_DIR)/parcer $(OBJ_DIR)/utils

clean:
	$(RM) $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR)
	@echo "$(CURRENT_COLOR)🧹 Archivos objeto eliminados.\n$(RESET)"

fclean: clean
	$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(CURRENT_COLOR)🧹 Binarios eliminados: $(NAME) y libft.a\n$(RESET)"

re: fclean all

#◉── Reglas adicionales ─────────────◉

norm:
	@echo "$(CURRENT_COLOR)➵ Ejecutando Norminette:$(RESET)"
	@norminette $(MINISHELL_SRCS) inc/minishell.h inc/datatypes.h inc/executor.h inc/tokenizer.h 

format: 
	c_formatter_42 $(MINISHELL_SRCS) inc/minishell.h inc/datatypes.h inc/executor.h inc/tokenizer.h 

.PHONY: all clean fclean re format norm
