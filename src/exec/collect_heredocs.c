#include "minishell.h"

int collect_all_heredocs(t_shell *shell)
{
    t_command *cmd;
    t_redir   *r;
    int       ret;

    cmd = shell->command;
    while (cmd)
    {
        r = cmd->redir;
        while (r)
        {
            if (r->type == REDIR_HEREDOC)
            {
                ret = collect_heredoc_fd(shell, r);
                if (ret != 0)
                    return ret;
            }
            r = r->next;
        }
        cmd = cmd->next;
    }
    return 0;
}
