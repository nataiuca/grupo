#◉────▣───▢◇▢───▣────◉•◉───▣───▢   Colors:   ▢───▣───◉•◉───▣────▢◇▢───▣───◉#

BLACK   := \033[0;30m
RED     := \033[0;31m
GREEN   := \033[0;32m
YELLOW  := \033[0;33m
BLUE    := \033[0;34m
MAGENTA := \033[0;35m
CYAN    := \033[0;36m

CURRENT_COLOR := \033[0;34m
RESET   := \033[0m

#◉───▣───▢◇▢───▣───◉•◉───▣───▢   Variables:   ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

NAME        := minishell
CC          := cc
CFLAGS      := -Wall -Werror -Wextra
RM          := rm -rf

READLINE_INC := -I/usr/local/opt/readline/include
READLINE_LIB := -L/usr/local/opt/readline/lib -lreadline

# Directories
SRC_DIR     := src
OBJ_DIR     := obj
INC_DIR     := inc


#◉───▣───▢◇▢───▣───◉•◉───▣───▢ Libft Variables ▢───▣───◉•◉───▣───▢◇▢───▣──◉#

LIBFT_DIR	:= libft/
LIBFT		:= $(LIBFT_DIR)/libft.a
INCLUDES_LIBFT := -I$(LIBFT_DIR)
INCLUDES_MINISHELL := -I$(INC_DIR)


#◉──▣──▢◇▢───▣──◉•◉───▣───▢ minishell includes ▢───▣───◉•◉───▣──▢◇▢───▣──◉#

INCLUDES_MINISHELL := -I$(INC_DIR)

#◉───▣───▢◇▢───▣───◉•◉───▣───▢ Minishell Sources ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

SRCS 					:= \
						main.c \
						\
						builtins/builtins.c \
						builtins/cd.c \
						builtins/echo.c \
						builtins/env.c \
						builtins/exit.c \
						builtins/export.c \
						builtins/export_utils.c \
						builtins/export_utils_2.c \
						builtins/pwd.c \
						builtins/unset.c \
						\
						\
						executor/built_args.c \
						executor/env_init.c \
						executor/env_utils.c \
						executor/executor.c \
						executor/executor_fork_utils.c \
						executor/executor_fork_cmd.c \
						executor/executor_fork_cmd_utils.c \
						executor/executor_single_cmd.c \
						executor/executor_utils.c \
						executor/executor_utils_2.c \
						executor/get_path_exec_utils.c \
						executor/get_path_execve.c \
						executor/here_doc.c \
						executor/here_doc_utils.c \
						executor/here_process.c \
						executor/pipes.c \
						executor/pipes_free.c \
						executor/redirections.c \
						executor/redirections_2.c \
						executor/redirections_utils.c \
						\
						parcer/var_check_expand.c \
						parcer/var_expand_core.c \
						parcer/var_lookup.c \
						\
						parcer/check_spaces_line.c \
						parcer/check_spaces_line_utils.c \
						\
						parcer/syntax_error_token_utils.c \
						parcer/syntax_error_token.c \
						\
						parcer/tokens_type_check_type.c \
						parcer/tokens_type_command_check.c \
						parcer/tokens_type_redir_check.c \
						parcer/tokens_type_pipe_check.c \
						\
						parcer/ckeck_quote_utils.c \
						parcer/free_utils.c \
						parcer/free_structs.c \
						parcer/ft_split_with_quotes.c \
						parcer/init_list.c \
						\
						parcer/init_structures.c \
						parcer/init_structures_utils.c \
						\
						parcer/quit_quotes_token.c \
						parcer/tokenizer.c \
						parcer/tokenizer_utils.c \
						parcer/get_prompt.c \
						parcer/handle_error.c \
						parcer/minishell_loop.c \
						\
						\
						utils/signals.c \
						utils/signals_2.c \
						utils/herequote_utils.c \

# Full .c paths
SRC_PATHS 			:= $(addprefix $(SRC_DIR)/,$(SRCS))

# Matching .o paths in OBJ_DIR
OBJS 				:= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

#◉───▣───▢◇▢───▣───◉•◉───▣───▢     rules      ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

all: $(LIBFT) $(NAME)

$(LIBFT): 
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(READLINE_LIB) -L$(LIBFT_DIR) -lft

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES_MINISHELL) $(INCLUDES_LIBFT) $(READLINE_INC) -c $< -o $@

$(OBJ_DIR): 
	@mkdir -p  $(OBJ_DIR) $(OBJ_DIR)/builtins $(OBJ_DIR)/executor $(OBJ_DIR)/parcer $(OBJ_DIR)/utils 

clean:
	$(RM) $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ) $(OBJ_DIR)
	@echo "$(CURRENT_COLOR)🧹 Object files cleaned.\n$(RESET)"

fclean: clean
	$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(CURRENT_COLOR) 🧹 $(NAME) and libft.a: binary cleaned.\n$(RESET)"

re: fclean all

.PHONY: all clean fclean re

