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

/*
**	We free `*line` and return -1 in case of an allocation error. Since `*line`
**	is always set to NULL at the beginning, no double free can happen.
*/
static int	gnl_error(char **line)
{
	free(*line);
	*line = NULL;
	return (-1);
}

/*
**	If we are at the end of the file, we check if there is something in `*line`,
**	if it's the case we just return 0, but if there is nothing in `*line`, we
**	still have to return an empty string.
*/
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

/*
**	We read BUFFER_SIZE characters from `fd` to our buffer while keeping track
**	of read's return value because it corresponds to the number of characters
**	that we read, so the length of the string inside the buffer. So we put
**	that value into `buf[fd].len` and null terminate the string.
*/
static void	gnl_read(int fd, t_buffer *buf)
{
	int	read_ret;

	read_ret = read(fd, buf->rd_buf, BUFFER_SIZE);
	buf->rd_buf[read_ret] = 0;
	buf->len = read_ret;
}

/*
**	Since we previously replaced the first '\n' by a '\0', we just have to call
**	`_ft_strjoin` to join the line and the buffer from the cursor to the first
**	null character (it can be either the first new line or the end of the buffer
**	if no new line has been found). After that we simply add the length of the
**	buffer from the cursor position to the first '\0', so
**	`_ft_strlen(&buf->rd_buf[buf->cursor]) + 1` to go to the next line.
*/
static int	gnl_update(char **line, t_buffer *buf)
{
	char	*tmp;

	tmp = _ft_strjoin(*line, &buf->rd_buf[buf->cursor]);
	if (!tmp)
		return (gnl_error(line));
	free(*line);
	*line = tmp;
	buf->cursor += _ft_strlen(&buf->rd_buf[buf->cursor]) + 1;
	return (0);
}

/*
**	1.	We start by initializing a static array of our buffer struct and we will
**		use our fd as index to be able to handle multiple fds at the same time.
**	2.	We check if either BUFFER_SIZE or `fd` are invalid.
**	3.	We set line to NULL to be sure that if we call free on it, it will not
**		free invalid memory, and the boolean `endl` to false, this variable will
**		indicate us if a new line has been found in the buffer.
**	4.	We keep looping until a new line has been found.
**
**	INSIDE THE LOOP
**	5.	If our cursor is equal to 0, it means that either we are at the
**		beginning of the file, or we reached the end of the buffer, so in both
**		cases it means that we have to read in the buffer. So we call
**		`gnl_read` (above), the return value of `read` will be in `buf[fd].len`.
**		If this value is equal to 0, that means we are at the end of the file,
**		in that case we call `gnl_eof` (above) and return 0.
**	8.	If a '\n' has been found, we replace it to '\0', then we call
**		`gnl_update` (above) to update `buf[fd].cursor` and `*line`.
**	9.	If `buf[fd].cursor` is superior or equal to `buf[fd].len`, it means
**		that we have read the entire buffer, so we set it back to 0.
**	10.	If we exit the loop, it means that we got a line, but we are not yet
**		at the end of the file (see step 5), so we return 1.
*/
int	get_next_line(int fd, char **line)
{
	static t_buffer	buf[MAX_FD];
	bool			endl;

	if (BUFFER_SIZE <= 0 || (fd != 0 && fd < 3))
		return (-1);
	*line = NULL;
	endl = false;
	while (!endl)
	{
		if (!buf[fd].cursor)
		{
			gnl_read(fd, &buf[fd]);
			if (!buf[fd].len)
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
