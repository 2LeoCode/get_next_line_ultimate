/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsuardi <lsuardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/04 15:45:43 by lsuardi           #+#    #+#             */
/*   Updated: 2021/04/06 21:10:35 by lsuardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <get_next_line.h>

static int	gnl_error(char **line)
{
	free(*line);
	*line = NULL;
	return (-1);
}

static int	gnl_eof(char **line, t_buffer *buf)
{
	if (*line)
		return (0);
	buf->cursor = 0;
	*(buf->rd_buf) = 0;
	*line = _ft_strdup("");
	if (!*line)
		return (-1);
	return (0);
}

static int	gnl_read(int fd, t_buffer *buf)
{
	int	read_ret;

	read_ret = read(fd, buf->rd_buf, BUFFER_SIZE);
	buf->rd_buf[read_ret] = 0;
	buf->len = read_ret;
	return (read_ret);
}

static int	gnl_update(char **line, t_buffer *buf)
{
	char	*tmp;

	tmp = _ft_strjoin(*line, &buf->rd_buf[buf->cursor]);
	if (!tmp)
		return (gnl_error(line));
	free(*line);
	*line = tmp;
	buf->cursor += (_ft_strlen(&buf->rd_buf[buf->cursor]) + 1);
	return (0);
}

int	get_next_line(int fd, char **line)
{
	static t_buffer	buf[MAX_FD];
	int				read_ret;
	int				endl;
	char			*tmp;

	if ((BUFFER_SIZE <= 0) || ((fd != 0) && (fd < 3)))
		return (-1);
	*line = NULL;
	endl = 0;
	while (!endl)
	{
		if (!buf[fd].cursor)
		{
			read_ret = gnl_read(fd, &buf[fd]);
			if (!read_ret)
				return (gnl_eof(line, &buf[fd]));
		}
		endl = _ft_str_replace_first(&buf[fd].rd_buf[buf[fd].cursor], '\n', 0);
		if (gnl_update(line, &buf[fd]))
			return (-1);
		if (buf[fd].cursor >= buf[fd].len)
			buf[fd].cursor = 0;
	}
	return (1);
}
