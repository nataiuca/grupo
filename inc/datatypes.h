/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   datatypes.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 08:43:48 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/26 22:23:25 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATATYPES_H
# define DATATYPES_H

//#➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Includes minishell:──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

# include "minishell.h"
# include <sys/types.h>

//#➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Macros_size:──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

# define HOSTNAME_SIZE				256
# define CWD_SIZE					1024
# define PROMPT_SIZE				2048
# define PATH_MAX					4096
# define HERE_DOCS_MAX				10
# define ARG_MAX_ECHO				((PROMPT_SIZE * 3)- 2)

//#➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Macros_error:──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

# define MSG_ERR_MALLOC				"error allocating memory"
# define MSG_ERR_CMD_NOT_FOUND		"command not found"
# define MSG_ERR_IS_DIRECTORY		"Is a directory"
# define MSG_ERR_NOT_IS_DIRECTORY	"Not a directory"
# define MSG_ERR_NO_SUCH_FILE		"No such file or directory"
# define MSG_ERR_PERMISSION_DENIED	"Permission denied"
# define MSG_ERR_EXEC_FORMAT		"Exec format error"
# define MSG_ERR_TOO_MANY_ARGS		"too many arguments"
# define MSG_ERR_NOT_NUMERIC		"numeric argument required"
# define MSG_ERR_INVALID_IDENTIFIER	"not a valid identifier"
# define MSG_ERR_FORK				"fork failed"
# define MSG_ERR_PIPE				"pipe failed"
# define MSG_ERR_MISS_ARG			"missing argument"
# define MSG_ERR_INV_OPT			"invalid option"
# define MSG_ERR_ARG_LEN			"Argument list too long"

//#➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Enums:──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

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
	heredoc_delimiter
} t_type;

//#➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Structures:──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

typedef struct s_env
{
	char			*key;				// ej: "PATH"
	char			*value;				// ej: "/usr/bin"
	int				is_export_only;		// 1 si fue export VAR sin '='
	struct s_env	*next;
}					t_env;

typedef struct s_program
{
	int				last_exit_status;	
//-------prompt-------//
	char			prompt_cwd[CWD_SIZE];	
	char			*prompt_user;			
	char			*prompt_hostname;		
	char			*prompt_home_path;		
	char			prompt[PROMPT_SIZE];	
//--------env--------//
	char			**envp_copy;		// **"split" (**matrix) de envp
	t_env			*env;				// lista enlazada del entorno								
}					t_program;

typedef struct s_tokens
{
	char			*content;			 // *str con datos		
	t_type			type;				// **type: command, argument, redir, etc 
	bool			expand_here;		// expandir o no here_doc	
	int				position;			// posicion del nodo en la lista	(revisar si hace falta) 
	bool			from_expansion;		// 🔧 Nuevo: viene de $VAR
	bool			was_quoted;			// 🔧 Nuevo: estaba entre '' o ""
	struct s_tokens	*next;			
}					t_tokens;

typedef struct s_exec
{
	int				**pipe_fds; // File descriptors para los pipes		
	int				infile;     // Descriptor de archivo de entrada		
	int				outfile;    // Descriptor de archivo de salida		
	int				num_pipes;  // num pipes							
	int				num_cmds;	// num comandos 						
	pid_t			*pids;    // ID del proceso hijo					
	bool			is_pipe;   // Indica si hay un pipe activo			
	char			**args;    // argc de execve						
}					t_exec;

typedef struct s_metachars
{
	//---------acces_to_program---------------------
	t_program		*program_acces;
	//---------parce_aux_splits---------------------
	char			**allow_op;        		// < > | << >> 		
	char			**var;             		// $				
	char			**unexpected_token;		//  `newline' `|' `>' `<' `>>' `<<'
	//---------parce_aux_vars---------------------
	int				i_index;    				//✅_se usa reservado para el uso de check_spaces_line() y otras funciones
	int				j_index;        			//✅_se usa reservado para el uso de check_spaces_line() y otras funciones
	int				x_index;					//✅_solo se usa en ft_split_with_quotes
	bool			in_single_quote;   			//✅_se usa reservado para el uso de check_spaces_line() y otras funciones
	bool			in_double_quote;   			//✅_se usa reservado para el uso de check_spaces_line() y otras funciones
	//---------type_flags---------------------
	bool			cmd_seen;
	bool			pipe_seen;
	bool			expecting_cmd;
	bool 			redir_pending;
}					t_metachars;

typedef struct s_here
{
	int				here_doc_num;								
	int				fd_array[HERE_DOCS_MAX];					
	char			*here_name_docs[HERE_DOCS_MAX];				
	char			*expanded_line_here; 					// con malloc	
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
	 bool        needs_more_input; 
}					t_all;


//#➵⤐╌╌➣⋆➣╌─⤏➵•➵⤐╌╌➣⋆➣╌╌─...──➣⋆➣╌⤏➵•➵⤐╌╌➣⋆➣╌╌➔#

//#➵⤐╌╌➣⋆➣╌─⤏➵•➵⤐╌╌➣⋆➣╌╌─Utilities──➣⋆➣╌⤏➵•➵⤐╌╌➣⋆➣╌╌➔#

// src/parcer/init_list.c (5/5) ✅_NORMA + ✅_DESCRIPCIONES + ⚠️_testeo + ✅_se usa
t_tokens			*find_last_node(t_tokens *tokens);
// static void		initialize_node(t_tokens *tokens, int position);
// static int		add_node(t_tokens **tokens, char *content, int position, t_program *program);
// void				init_list(t_tokens **tokens, char **split_str);
void				init_list(t_tokens **tokens, char **split_str, t_program *program);


//#➵⤐╌╌➣⋆➣╌─⤏➵•➵⤐╌╌➣⋆➣╌╌─Environment──➣⋆➣╌⤏➵•➵⤐╌╌➣⋆➣╌╌➔#

// src/parcer/init_structures.c (5/5) // ⚠️_NORMA + ⚠️_testeo + ✅_DESCRIPCIONES + ✅_se usa
void				init_program(t_program *program, char **envp);
t_exec				*init_exec(t_program *program);
t_metachars			*init_meta_struct(t_program *program);
t_here				*init_here_struct(t_program *program);
bool				init_all_structs(t_all *all, t_program *program);

// src/parcer/init_structures_utils.c // ⚠️_NORMA + ⚠️_testeo + ✅_DESCRIPCIONES + ✅_se usa
bool				init_meta_splits(t_metachars *meta, t_program *program);
//--STRUCTS_END──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#



//--FREE──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#
// src/parcer/free_structs.c (/5)	// ⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES 
void				free_exec(t_exec *exec);
void				free_meta_struct(t_metachars *meta);
void				free_here(t_here **ph);
void				free_program(t_program *program);
void				free_all_structs(t_all *all);

// /src/parcer/free_utils.c (/5)	// ⚠️_NORMA + ⚠️_testeo + ⚠️_DESCRIPCIONES 
//--------------------- FREE EXEC ---------------------//
void				cleanup_exec(t_exec *exec);
void				destroy_env_list(t_env *env);
//void				clear_matrix_ptr(char ***matrix); // ⚠️_revisar si se usa//
//void				ft_free_matrix(char **matrix);
void				destroy_env_node(t_env *node);
//--------------------- FREE SPLIT---------------------//
void				free_split_strs(char **split_strs);
//--------------------- FREE TOKENS---------------------//
void				free_tokens_list(t_tokens **tokens);
//--------------------- FREE CHILD AND EXIT---------------------//
void				free_child (t_program *program, t_all *all);
//--FREE_END──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#


#endif