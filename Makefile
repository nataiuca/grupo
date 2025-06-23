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
CFLAGS      := -Wall -Werror -Wextra -g
RM          := rm -rf

READLINE_FLAGS			:=	-lreadline

# Directories
SRC_DIR     := src
OBJ_DIR     := obj
INC_DIR     := inc


#◉───▣───▢◇▢───▣───◉•◉───▣───▢ Libft Variables ▢───▣───◉•◉───▣───▢◇▢───▣──◉#

LIBFT_DIR	:= libft/
LIBFT		:= $(LIBFT_DIR)/libft.a
INCLUDES_LIBFT := -I$(LIBFT_DIR)


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
						executor/executor_single_cmd.c \
						executor/executor.c \
						executor/executor_loop.c \
						executor/get_path_execve.c \
						executor/here_doc.c \
						executor/here_doc_utils.c \
						executor/executor_utils.c \
						executor/built_args.c \
						executor/redirections.c \
						executor/env_init.c \
						executor/env_utils.c \
						executor/pipes.c \
						\
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
						parcer/tokens_type_redir_context_check.c \
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
						utils/herequote_utils.c \
						\
						test_utils.c \
						\

# Full .c paths
SRC_PATHS 			:= $(addprefix $(SRC_DIR)/,$(SRCS))

# Matching .o paths in OBJ_DIR
OBJS 				:= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

#◉───▣───▢◇▢───▣───◉•◉───▣───▢     rules      ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

all: $(LIBFT) $(NAME)

$(LIBFT): 
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) $(READLINE_FLAGS) -lft 
	@echo "$(CURRENT_COLOR) ✅ $(NAME) created.\n$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
#	@echo "$(CYAN)🔧 Compiling: $<$(RESET)"
	$(CC) $(CFLAGS) $(INCLUDES_MINISHELL) $(INCLUDES_LIBFT) -c $< -o $@

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

#◉───▣───▢◇▢───▣───◉•◉───▣───▢   Valgrind   ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

VALGRIND := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-limit=no --log-file=dev/test/valgrind_outputs/valgrind_output_$(shell date +%Y%m%d_%H%M%S).log -s

valgrind: all

	@echo "$(CURRENT_COLOR)➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Running Valgrind..──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──$(RESET)"
	@-$(VALGRIND) ./$(NAME)
	@mkdir -p dev/test/valgrind_outputs
	@dev/test/hs_files/./open_valgrind_log.sh
	@echo "$(CURRENT_COLOR)➵⤐╌╌➣⋆➣╌─⤏➵•➵⤐─╌╌➣⋆➣── Valgrind completed. Check valgrind_output.log for details. ─╌➣⋆➣╌─⤏➵•➵⤐─╌╌➣⋆➣╌╌─$(RESET)"
	
clean_valgrind:
	dev/test/hs_files/./clean_valgrind.sh


#◉───▣───▢◇▢───▣───◉•◉───▣───▢ Sanitizers ▢───▣───◉•◉───▣───▢◇▢───▣───◉#
#◉──▢◇▢───▣───◉•◉────▢Sanitizer  All▢────◉•◉───▣───▢◇▢──◉#

san_all: run_asan run_lsan run_tsan run_usan

#◉──▢◇▢───▣───◉•◉────▢AddressSanitizer ▢────◉•◉───▣───▢◇▢──◉#

asan: $(PIPEX_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -fsanitize=address -g $(MINISHELL_SRCS) -o $(NAME) -L$(LIBFT_DIR) -lft $(INCLUDES_LIBFT)
	@echo "$(CURRENT_COLOR) $(NAME) compiled with AddressSanitizer.\n$(RESET)"

run_asan: asan
	@echo "$(CURRENT_COLOR)➵ Running with AddressSanitizer: $(RESET)"
	./$(NAME) echo hola | grep hola
	@echo "$(CURRENT_COLOR)➵ AddressSanitizer run completed.$(RESET)"

#◉──▢◇▢───▣───◉•◉────▢UndefinedBehaviorSanitizer ▢────◉•◉───▣───▢◇▢──◉#

usan: $(PIPEX_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -fsanitize=undefined -g $(MINISHELL_SRCS) -o $(NAME) -L$(LIBFT_DIR) -lft $(INCLUDES_LIBFT)
	@echo "$(CURRENT_COLOR) $(NAME) compiled with UndefinedBehaviorSanitizer.\n$(RESET)"

run_usan: usan
	@echo "$(CURRENT_COLOR)➵ Running with UndefinedBehaviorSanitizer: $(RESET)"
	./$(NAME)  echo hola | grep hola
	@echo "$(CURRENT_COLOR)➵ UndefinedBehaviorSanitizer run completed.$(RESET)"

#◉──▢◇▢───▣───◉•◉────▢LeakSanitizer ▢────◉•◉───▣───▢◇▢──◉#

lsan: $(PIPEX_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -fsanitize=leak -g $(MINISHELL_SRCS) -o $(NAME) -L$(LIBFT_DIR) -lft $(INCLUDES_LIBFT)
	@echo "$(CURRENT_COLOR) $(NAME) compiled with LeakSanitizer.\n$(RESET)"

run_lsan: lsan
	@echo "$(CURRENT_COLOR)➵ Running with LeakSanitizer: $(RESET)"
	./$(NAME)  echo hola | grep hola
	@echo "$(CURRENT_COLOR)➵ LeakSanitizer run completed.$(RESET)"

#◉──▢◇▢───▣───◉•◉────▢ThreadSanitizer ▢────◉•◉───▣───▢◇▢──◉#

tsan: $(PIPEX_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -fsanitize=thread -g $(MINISHELL_SRCS) -o $(NAME) -L$(LIBFT_DIR) -lft $(INCLUDES_LIBFT)
	@echo "$(CURRENT_COLOR) $(NAME) compiled with ThreadSanitizer.\n$(RESET)"

run_tsan: tsan
	@echo "$(CURRENT_COLOR)➵ Running with ThreadSanitizer: $(RESET)"
	./$(NAME)  echo hola | grep hola
	@echo "$(CURRENT_COLOR)➵ ThreadSanitizer run completed.$(RESET)"


#◉───▣───▢◇▢───▣───◉•◉───▣───▢Norminette ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

norm:
	@echo "$(CURRENT_COLOR)➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌── Checking Norminette: ──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔$(RESET)"
	@norminette $(MINISHELL_SRCS) inc/minishell.h inc/datatypes.h inc/executor.h inc/tokenizer.h 
	@echo "$(CURRENT_COLOR)➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌── Norminette completed. Check your output for details. ─╌➣⋆➣╌─⤏➵•➵⤐─╌╌➣⋆➣╌╌─$(RESET)"

#◉───▣───▢◇▢───▣───◉•◉───▣───▢  Format ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

format: 
	c_formatter_42 $(MINISHELL_SRCS) inc/minishell.h inc/datatypes.h inc/executor.h inc/tokenizer.h 

#◉───▣───▢◇▢───▣───◉•◉───▣───▢ Phony targets ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

.PHONY: all clean fclean re format norm valgrind clean_valgrind san_all run_asan run_usan run_lsan run_tsan 