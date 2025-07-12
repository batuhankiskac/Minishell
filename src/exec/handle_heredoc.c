/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/11 19:34:19 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Yardımcı fonksiyon: Heredoc okuma ve genişletme döngüsü
static int	read_heredoc_input(t_shell *shell, t_redir *redir, int temp_fd)
{
	char	*line;
	char	*expanded_line;

	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);

		// Ctrl+C ile kesinti olduysa veya dosya sonuna ulaşıldıysa
		if (line == NULL)
		{
			write(1, "\n", 1);
			return (ERROR);
		}

		// Delimiter (sınırlayıcı) ile karşılaştır
		if (ft_strncmp(line, redir->file, ft_strlen(redir->file)) == 0
			&& line[ft_strlen(redir->file)] == '\n')
		{
			free(line);
			break; // Başarıyla tamamlandı.
		}

		// *** GENİŞLETME MANTIĞI BURADA ***
		if (redir->expand_content)
		{
			// Eğer delimiter tırnak içinde değilse, değişkenleri genişlet.
			expanded_line = expand_string(line, shell->env, shell->exit_status);
			free(line); // Orijinal satırı serbest bırak
			if (!expanded_line)
				return (ERROR); // Genişletme sırasında hata olursa
			write(temp_fd, expanded_line, ft_strlen(expanded_line));
			free(expanded_line);
		}
		else
		{
			// Eğer delimiter tırnak içindeyse, satırı olduğu gibi yaz.
			write(temp_fd, line, ft_strlen(line));
			free(line);
		}
	}
	return (0);
}

int	handle_heredoc_redir(t_shell *shell)
{
	t_command	*cmd;
	t_redir		*redir;
	int			temp_fd;

	cmd = shell->command;
	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			// Sadece TOKEN_HEREDOC ile ilgileniyoruz, REDIR_IN'e dönüşmüş olanlarla değil.
			// Bu yüzden parser'daki dönüşümü geri alabilir veya burada kontrol ekleyebiliriz.
			// Şimdilik, parser'ın heredoc'u REDIR_IN'e çevirmediğini varsayalım.
			if (redir->type == REDIR_HEREDOC)
			{
				temp_fd = open(".heredoc_temp", O_CREAT | O_WRONLY | O_TRUNC, 0600);
				if (temp_fd == -1)
					return (print_error(NULL, "heredoc", strerror(errno), ERROR));

				if (read_heredoc_input(shell, redir, temp_fd) == ERROR)
				{
					close(temp_fd);
					unlink(".heredoc_temp");
					return (ERROR);
				}

				close(temp_fd);

				// Artık bu heredoc, normal bir dosya girdisi gibi davranacak.
				// Orijinal parser mantığınıza geri dönelim: heredoc_fd'yi kullanalım.
				redir->heredoc_fd = open(".heredoc_temp", O_RDONLY);
				if (redir->heredoc_fd == -1)
				{
					unlink(".heredoc_temp");
					return (print_error(NULL, ".heredoc_temp", strerror(errno), ERROR));
				}
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
