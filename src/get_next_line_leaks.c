/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-amra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 16:09:27 by iel-amra          #+#    #+#             */
/*   Updated: 2021/11/30 17:23:11 by iel-amra         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_leaks.h"
#include <fcntl.h>

static t_list	*ft_lstnew(int len)
{
	t_list	*elem;

	elem = malloc(sizeof(t_list));
	if (!elem)
		return ((void *) 0);
	elem->content = malloc(len);
	if (!elem->content)
	{
		free(elem);
		return ((void *) 0);
	}
	elem->next = ((void *) 0);
	return (elem);
}

static int	ft_lstsize(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

static int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] && (char)c != s[i])
		i++;
	if ((char)c == s[i])
		return ((char *)s + i);
	return ((void *) 0);
}

static void	ft_lstclear(t_list **lst, void (*del)(void*), int mode)
{
	t_list	*next;
	t_list	*current;

	current = *lst;
	while ((current || mode) && (current->next || !mode))
	{
		next = current->next;
		(*del)(current->content);
		free(current);
		current = next;
	}
	*lst = current;
}

static int	read_in_t_list(t_list *my_strings, int fd)
{
	int		read_return;
	t_list	*travel;

	read_return = 1;
	travel = my_strings;
	while (!ft_strchr(travel->content, '\n') && read_return > 0)
	{
		travel->next = ft_lstnew((BUFFER_SIZE + 1) * sizeof(char));
		if (!travel->next)
		{
			ft_lstclear(&my_strings, free, 0);
			return (1);
		}
		travel = travel->next;
		read_return = read(fd, travel->content, BUFFER_SIZE);
		if (read_return == -1)
		{
			ft_lstclear(&my_strings, free, 0);
			return (1);
		}
		travel->content[read_return] = '\0';
	}
	return (0);
}

static char	*ft_custom_strcpy(char *dst, char *src, int mode)
{
	int	i;

	i = 0;
	while (src[i] && (src[i] != '\n' || mode == 0))
	{
		dst[i] = src[i];
		i++;
	}
	if (src[i] == '\n')
	{
		dst[i] = '\n';
		i++;
	}
	dst[i] = '\0';
	return (dst + i);
}

static char	*malloc_final(t_list *my_strings)
{
	int		len;
	char	*sp_pos;

	len = 1;
	while (my_strings)
	{
		sp_pos = ft_strchr(my_strings->content, '\n');
		if (!sp_pos)
			len += ft_strlen(my_strings->content);
		else
			len += sp_pos - my_strings->content + 1;
		my_strings = my_strings->next;
	}
	if (len == 1)
		return ((void *) 0);
	return (malloc(len));
}

static char	*get_final(t_list *my_strings)
{
	char	*final;
	char	*travel;

	final = malloc_final(my_strings);
	if (!final)
	{
		ft_lstclear(&my_strings, free, 0);
		return ((void *) 0);
	}
	travel = final;
	while (my_strings)
	{
		travel = ft_custom_strcpy(travel, my_strings->content, 1);
		my_strings = my_strings->next;
	}
	return (final);
}

char	*get_next_line_leaks(int fd)
{
	static char	ending[OPEN_MAX][BUFFER_SIZE];
	t_list		*my_strings;
	char		*final;

	if (fd > OPEN_MAX || fd < 0 || BUFFER_SIZE <= 0)
		return ((void *) 0);
	my_strings = ft_lstnew((ft_strlen(ending[fd]) + 1) * sizeof(char));
	if (!my_strings)
		return ((void *) 0);
	ft_custom_strcpy(my_strings->content, ending[fd], 0);
	if (read_in_t_list(my_strings, fd))
		return ((void *) 0);
	final = get_final(my_strings);
	if (!final)
		return ((void *) 0);
	ft_lstclear(&my_strings, free, 1);
	if (ft_strchr(my_strings->content, '\n'))
		ft_custom_strcpy(ending[fd],
			ft_strchr(my_strings->content, '\n') + 1, 0);
	else
		ft_custom_strcpy(ending[fd], "\0", 0);
	ft_lstclear(&my_strings, free, 0);
	return (final);
}
