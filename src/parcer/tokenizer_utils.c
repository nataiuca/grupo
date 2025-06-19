/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 15:00:15 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/07 13:19:26 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
int get_or_set_last_exit_status(int new_status, bool set)
{
    static int last_status=0;

	//last_status = 0;
    if (set)
        last_status = new_status;
    return (last_status);
}
	*/
int get_or_set_last_exit_status(int new_status, bool set)
{
	static int last_status;
	static bool initialized;

	if (!initialized)
	{
		last_status = 0;
		initialized = true;
	}
	if (set)
		last_status = new_status;
	return (last_status);
}


/*
char *get_line(void)
{
	size_t len;
	char *line;
	char *temp_line;
	
	temp_line = get_next_line(0);
	if (!temp_line) 
	{
		printf("Error: get_line returned NULL\n");  //quitar comment
		return (NULL);
	}
	len = ft_strlen(temp_line);
	if (len > 0 && temp_line[len - 1] == '\n') 
	{
		len--;  // Ignorar el salto de línea
	}
	line = (char *)malloc(len + 1);
	if (!line) 
	{
		free(temp_line);
		printf("Error: Memory allocation failed\n");  //quitar comment
		return (NULL);
	}
	ft_strlcpy(line, temp_line, len + 1); 
	free (temp_line);
	return (line);
}
*/

char *get_line_prompt(t_program *program) // 🚩 solo se usa en dev en vez de readline
{
	size_t	len;
	char	*line;
	char	*temp_line;

	// Mostrar el prompt como readline
	ft_putstr_fd(get_prompt(program), 1); // stdout

	temp_line = get_next_line(0);
	if (!temp_line) 
	{
		printf("Error: get_line returned NULL\n");
		return (NULL);
	}

	len = ft_strlen(temp_line);
	if (len > 0 && temp_line[len - 1] == '\n') 
		len--;  // Ignorar el salto de línea

	line = (char *)malloc(len + 1);
	if (!line) 
	{
		free(temp_line);
		printf("Error: Memory allocation failed\n");
		return (NULL);
	}

	ft_strlcpy(line, temp_line, len + 1);
	free(temp_line);

	return (line);
}

char *get_line_str(char *str, t_program *program) //✅_se usa en here_doc
{
    size_t	len;
    char	*line;
    char	*temp_line;

    ft_putstr_fd(str, 1); // Mostrar el string como prompt por stdout
    temp_line = get_next_line(0); // Leer del stdin (fd = 0)
    if (!temp_line) 
    {
        return (ft_error(program, MSG_ERR_MALLOC, NULL, 1), NULL);
    }
    len = ft_strlen(temp_line);
    if (len > 0 && temp_line[len - 1] == '\n') 
        len--;  // Ignorar el salto de línea
    line = (char *)malloc(len + 1);
    if (!line) 
    {
        free(temp_line);
        return (ft_error(program, MSG_ERR_MALLOC, NULL, 1), NULL);
    }
    ft_strlcpy(line, temp_line, len + 1);
    free(temp_line);
    return (line);
}

t_tokens	*ft_find_node_n_position(t_tokens *head, int position)
{
	while (head != NULL)
	{
		if (head->position == position)
			return (head);
		head = head->next;
	}
	return (NULL);
}

bool ft_str_is_empty_or_spaces(const char *str)
{
	if (!str)
		return true;

	while (*str)
	{
		if (!ft_isspace(*str))
			return (false);
		str++;
	}
	return (true);
}

void reset_meta_flags(t_metachars *meta, int flag)
{
    if (flag == 1)
    {
        meta->i_index = 0;
        meta->j_index = 0;
    }
    else if (flag == 2)
    {
        meta->in_single_quote = false;
        meta->in_double_quote = false;
    }
    else if (flag == 3)
    {
        meta->i_index = 0;
        meta->j_index = 0;
        meta->x_index = 0;
        meta->in_single_quote = false;
        meta->in_double_quote = false;
    }
}
