/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 08:44:49 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/06 12:29:00 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//#➵⤐──╌╌➣⋆➣── Includes C ────────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

# include <curses.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

//#➵⤐──╌╌➣⋆➣── Includes minishell ──➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌╌╌➔#

# include "datatypes.h"
# include "executor.h"
# include "tokenizer.h"

//#➵⤐──╌╌➣⋆➣── Includes libft ────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

# include "../libft/libft.h"

//#➵⤐──╌╌➣⋆➣── Colors ────────────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

# define ANSI_BLUE "\001\033[1;34m\002"
# define ANSI_RED "\001\033[1;31m\002"
# define ANSI_CGRAY_DARK "\001\x1B[90m\002"
# define ANSI_RESET "\001\033[0m\002"

//#➵⤐──╌╌➣⋆➣── Globals ───────────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

extern volatile sig_atomic_t	g_atomic;

//#➵⤐──╌╌➣⋆➣── Prototipes ────────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

// src/main.c
// int main (int argc, char *argv[], char **envp);

//#➵⤐──╌╌➣⋆➣── Signals ───────────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

// src/utils/signals.c
void							handler(int signum);
void							signal_handling(void);
void							handler_child(int signum);
void							handler_herequote(int signum);

// src/utils/signals_2.c
void							handler_builtins(int signum);
bool							catch_ctrl_d(t_program *program, t_all *all,
									char *input);

// src/utils/herequote_utils.c
char							*herequote_hook_rl(t_program *program);
int								herequote_check_g_atomic(t_program *program,
									char *here_line);
int								event_hook(void);

#endif