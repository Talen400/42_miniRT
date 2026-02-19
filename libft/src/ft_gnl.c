/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gnl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 14:10:48 by rgregori          #+#    #+#             */
/*   Updated: 2025/07/16 10:47:18 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_fd_node	*ft_find_and_creat(t_fd_node **head, int fd)
{
	t_fd_node	*node;

	node = *head;
	while (node)
	{
		if (node->fd == fd)
			return (node);
		node = node->next;
	}
	node = malloc(sizeof(t_fd_node));
	if (!node)
		return (NULL);
	node->fd = fd;
	node->stash = ft_strdup("");
	node->next = *head;
	*head = node;
	return (node);
}

static int	ft_read_and_update_stash(t_fd_node *node, int fd, char *buffer)
{
	int		bytes_read;
	char	*tmp;
	char	*newline_ptr;

	while (1)
	{
		newline_ptr = node->stash;
		while (*newline_ptr && *newline_ptr != '\n')
			newline_ptr++;
		if (*newline_ptr == '\n')
			break ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (-1);
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		tmp = ft_strjoin(node->stash, buffer);
		if (!tmp)
			return (-1);
		free(node->stash);
		node->stash = tmp;
	}
	return (0);
}

static char	*ft_get_line_from_stash(char *stash)
{
	int		i;

	if (!stash)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	return (ft_substr(stash, 0, i));
}

static void	ft_free_fd_node(t_fd_node **head, int fd)
{
	t_fd_node	*curr;
	t_fd_node	*prev;

	curr = *head;
	prev = NULL;
	while (curr)
	{
		if (curr->fd == fd)
		{
			if (prev)
				prev->next = curr->next;
			else
				*head = curr->next;
			free(curr->stash);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

char	*get_next_line(int fd)
{
	static t_fd_node	*head = NULL;
	t_fd_node			*node;
	char				*line;
	char				*new_stash;
	char				*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	node = ft_find_and_creat(&head, fd);
	if (!node || ft_read_and_update_stash(node, fd, buffer) == -1)
		return (free(buffer), ft_free_fd_node(&head, fd), NULL);
	if (!node->stash || !*node->stash)
		return (free(buffer), ft_free_fd_node(&head, fd), NULL);
	line = ft_get_line_from_stash(node->stash);
	if (!line)
		return (free(buffer), ft_free_fd_node(&head, fd), NULL);
	new_stash = ft_substr(node->stash, ft_strlen(line),
			ft_strlen(node->stash) - ft_strlen(line));
	free(node->stash);
	node->stash = new_stash;
	free(buffer);
	return (line);
}
