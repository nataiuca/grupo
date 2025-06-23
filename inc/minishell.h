/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 08:44:49 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/21 07:51:13 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//#➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Includes C:──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

# include <curses.h>			// tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <dirent.h>			// opendir, readdir, closedir
# include <errno.h>				// perror
# include <fcntl.h>				// open
# include <readline/history.h>	
# include <readline/readline.h> // readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history
# include <signal.h>            // signal, sigaction, kill
# include <stdbool.h>           //bool
# include <stdio.h>             // printf
# include <stdlib.h>            // malloc, free, getenv
# include <string.h>            // strerror
# include <sys/stat.h>          // stat, lstat, fstat
# include <sys/types.h>
# include <sys/wait.h>          // wait, waitpid, wait3, wait4
# include <termios.h>           // tcsetattr, tcgetattr
# include <unistd.h>            // write, access, read, close, fork, execve,pipe, isatty
# include <termios.h>


//#➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Includes minishell:──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

# include "datatypes.h"
# include "tokenizer.h"
# include "executor.h"

//#➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Includes libft:──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

# include "../libft/libft.h"

//#➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Colors:──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

# define ANSI_BLUE			"\001\033[1;34m\002"
# define ANSI_RED			"\001\033[1;31m\002"
# define ANSI_CGRAY_DARK	"\001\x1B[90m\002"
# define ANSI_RESET			"\001\033[0m\002"

//#➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Globals:──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

extern volatile sig_atomic_t	g_atomic; 

//#➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Prototypes:──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

// src/main.c ⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES
// int main (int argc, char *argv[], char **envp);


// src/utils/signals.c (/5)  ⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES
void				handler(int signum);
void				signal_handling(void);
void				handler_child(int signum);				//child handler
void				handler_herequote(int signum);		 //here handler
void				handler_builtins(int signum);
bool				catch_interactive(t_program *program, t_all *all, char *input);



// src/utils/signals.c (/5)  ⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES
char				*herequote_hook_rl(t_program *program);
int					herequote_check_g_atomic(t_program *program, char *here_line);
int					event_hook(void);


// src/test_utils.c (borrar)
void				print_metachars(const t_metachars *meta);
void				print_string_array(const char *label, char **array);
void				print_linked_list(t_tokens *head, int fd);
void				print_type_list(t_tokens *tokens, int fd);
void				print_content_list(t_tokens *tokens, int fd);
void				print_list(t_tokens *tokens, int fd);
void				print_split_before_list(char **token, int fd);
void				print_command_line(char *line, int fd);
void				print_argv_exec(char **argv, int fd);
void				print_all_test(char *line, char **token, t_tokens *tokens, int fd);
void				print_all_test_2(char *line, t_tokens *tokens, t_all *all, int fd);
void				print_all_test_3(char *line, t_tokens *tokens, int fd);


#endif

//#➵⤐╌╌➣⋆➣╌─⤏➵•➵⤐╌╌➣⋆➣╌╌─Core Functions──➣⋆➣╌⤏➵•➵⤐╌╌➣⋆➣╌╌➔#
//#➵⤐╌╌➣⋆➣╌─⤏➵•➵⤐╌╌➣⋆➣╌╌─Prompt──➣⋆➣╌⤏➵•➵⤐╌╌➣⋆➣╌╌➔#
//#➵⤐╌╌➣⋆➣╌─⤏➵•➵⤐╌╌➣⋆➣╌╌─Parsing──➣⋆➣╌⤏➵•➵⤐╌╌➣⋆➣╌╌➔#
//#➵⤐╌╌➣⋆➣╌─⤏➵•➵⤐╌╌➣⋆➣╌╌─Signals ──➣⋆➣╌⤏➵•➵⤐╌╌➣⋆➣╌╌➔#
//#➵⤐╌╌➣⋆➣╌─⤏➵•➵⤐╌╌➣⋆➣╌╌─Environment──➣⋆➣╌⤏➵•➵⤐╌╌➣⋆➣╌╌➔#
//#➵⤐╌╌➣⋆➣╌─⤏➵•➵⤐╌╌➣⋆➣╌╌─Execution──➣⋆➣╌⤏➵•➵⤐╌╌➣⋆➣╌╌➔#
//#➵⤐╌╌➣⋆➣╌─⤏➵•➵⤐╌╌➣⋆➣╌╌─Builtins──➣⋆➣╌⤏➵•➵⤐╌╌➣⋆➣╌╌➔#
//#➵⤐╌╌➣⋆➣╌─⤏➵•➵⤐╌╌➣⋆➣╌╌─Utilities──➣⋆➣╌⤏➵•➵⤐╌╌➣⋆➣╌╌➔#
//#➵⤐╌╌➣⋆➣╌─⤏➵•➵⤐╌╌➣⋆➣╌╌─...──➣⋆➣╌⤏➵•➵⤐╌╌➣⋆➣╌╌➔#