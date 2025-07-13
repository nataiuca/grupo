/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   datatypes.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 08:43:48 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/07 13:51:56 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATATYPES_H
# define DATATYPES_H

//#➵⤐──╌╌➣⋆➣── Includes minishell ──➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌╌╌➔#

# include "minishell.h"
# include <sys/types.h>

//#➵⤐──╌╌➣⋆➣── Define_loop ───────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

# define INPUT_CONTINUE 0
# define INPUT_BREAK 1
# define INPUT_ERROR -1

//#➵⤐──╌╌➣⋆➣── Macros_size ───────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

# define HOSTNAME_SIZE 256
# define CWD_SIZE 1024
# define PROMPT_SIZE 2048
# define PATH_MAX 4096
# define HERE_DOCS_MAX 10
# define ARG_MAX_ECHO 6140

//#➵⤐──╌╌➣⋆➣── Define_free ───────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

# define FREE_ALL 0
# define FREE_CHILD 1
# define FREE_ONLY_HEREDOC 2

//#➵⤐──╌╌➣⋆➣── Define_here ───────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

# define HEREDOC_OK 0
# define HEREDOC_SIGNAL -1
# define HEREDOC_ALLOC_FAIL -2
# define HEREDOC_CONTINUE 2

//#➵⤐──╌╌➣⋆➣── Define_error ──────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

# define MSG_ERR_MALLOC "error allocating memory"
# define MSG_ERR_CMD_NOT_FOUND "command not found"
# define MSG_ERR_IS_DIRECTORY "Is a directory"
# define MSG_ERR_NOT_IS_DIRECTORY "Not a directory"
# define MSG_ERR_NO_SUCH_FILE "No such file or directory"
# define MSG_ERR_PERMISSION_DENIED "Permission denied"
# define MSG_ERR_EXEC_FORMAT "Exec format error"
# define MSG_ERR_TOO_MANY_ARGS "too many arguments"
# define MSG_ERR_NOT_NUMERIC "numeric argument required"
# define MSG_ERR_INVALID_IDENTIFIER "not a valid identifier"
# define MSG_ERR_FORK "fork failed"
# define MSG_ERR_PIPE "pipe failed"
# define MSG_ERR_MISS_ARG "missing argument"
# define MSG_ERR_INV_OPT "invalid option"
# define MSG_ERR_ARG_LEN "Argument list too long"

//#➵⤐──╌╌➣⋆➣── Enums ─────────────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

typedef enum e_type
{
	temp,
	command,
	argument,
	redir_l_u,
	redir_l_d,
	redir_r_u,
	redir_r_d,
	pipe_type,
	path_name,
	here_d
}					t_type;

//#➵⤐──╌╌➣⋆➣── Structures ─────────────➣⋆➣╌╌──⤏➵•➵⤐─╌╌➣⋆➣╌╌➔#

typedef struct s_env
{
	char			*key;
	char			*value;
	int				is_export_only;
	struct s_env	*next;
}					t_env;

typedef struct s_program
{
	int				last_exit_status;
	int				input_status;
	char			prompt_cwd[CWD_SIZE];
	char			*prompt_user;
	char			*prompt_hostname;
	char			*prompt_home_path;
	char			prompt[PROMPT_SIZE];
	char			**envp_copy;
	t_env			*env;
}					t_program;

typedef struct s_tokens
{
	char			*content;
	t_type			type;
	bool			expand_here;
	int				position;
	bool			from_expansion;
	bool			was_quoted;
	struct s_tokens	*next;
}					t_tokens;

typedef struct s_exec
{
	int				**pipe_fds;
	int				infile;
	int				outfile;
	int				num_pipes;
	int				num_cmds;
	pid_t			*pids;
	bool			is_pipe;
	char			**args;
}					t_exec;

typedef struct s_metachars
{
	t_program		*program_acces;
	char			**allow_op;
	char			**var;
	char			**unexpected_token;
	int				i_index;
	int				j_index;
	int				x_index;
	bool			in_single_quote;
	bool			in_double_quote;
	bool			cmd_seen;
	bool			pipe_seen;
	bool			expecting_cmd;
	bool			redir_pending;
}					t_metachars;

typedef struct s_here
{
	int				here_doc_num;
	char			*here_line;
	int				here_c;
	int				fd_array[HERE_DOCS_MAX];
	char			*here_name_docs[HERE_DOCS_MAX];
	char			*expanded_line_here;
	char			*base_cwd;
}					t_here;

typedef struct s_all
{
	t_exec			*exec;
	t_metachars		*meta;
	t_here			*here;
	t_tokens		*tokens;
	t_tokens		*tokens_head;
	char			**token;
	char			*line;
}					t_all;

typedef struct s_hstate
{
	bool			delim_reached;
	int				lineno;
}					t_hstate;

//#➵⤐──╌╌➣⋆➣── Utilities ──────────────➣⋆➣╌╌──⤏➵•➵⤐─╌╌➣⋆➣╌╌➔#

// src/parcer/init_list.c (5/5)
t_tokens			*find_last_node(t_tokens *tokens);
// static void		initialize_node(t_tokens *tokens, int position);
// static int		add_node(t_tokens **tokens, char *content, int position,
//			t_program *program);
// void				init_list(t_tokens **tokens, char **split_str);
void				init_list(t_tokens **tokens, char **split_str,
						t_program *program);

//#➵⤐──╌╌➣⋆➣── Environment ────────────➣⋆➣╌╌──⤏➵•➵⤐──╌➣⋆➣╌╌➔#

// src/parcer/init_structures.c
void				init_program(t_program *program, char **envp);
t_exec				*init_exec(t_program *program);
t_metachars			*init_meta_struct(t_program *program);
t_here				*init_here_struct(t_program *program);
bool				init_all_structs(t_all *all, t_program *program);

// src/parcer/init_structures_utils.c
bool				init_meta_splits(t_metachars *meta, t_program *program);
void				free_split_strs(char **split_strs);
void				free_tokens_list(t_tokens **tokens);
void				free_child(t_program *program, t_all *all);

// src/parcer/free_structs.c
void				free_exec(t_exec *exec);
void				free_meta_struct(t_metachars *meta);
void				free_here(t_here *here, int mode);
void				free_program(t_program *program);
void				free_all_structs(t_all *all, int mode);

// src/parcer/free_utils.c
void				cleanup_exec_pipes(t_exec *exec);
void				cleanup_exec(t_exec *exec);
void				destroy_env_list(t_env *env);
void				free_all_exec_meta_tokens(t_all *all);
void				destroy_env_node(t_env *node);

#endif