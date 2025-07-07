# Valgrind Leak Report: `cat << EOF` (Interactive Mode)

**Date:** July 7, 2025
**Command Executed:** `echo -e "cat << EOF\nhello heredoc\nEOF" | valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes ./minishell`

## Summary

The Valgrind analysis for the command `cat << EOF` executed in interactive mode shows:

*   **Definitely lost:** 8 bytes in 2 blocks
*   **Indirectly lost:** 0 bytes in 0 blocks
*   **Possibly lost:** 0 bytes in 0 blocks
*   **Still reachable:** 232,888 bytes in 237 blocks

## Analysis of Leaks Originating from Minishell Code

### Leak 1: Definitely Lost Bytes from `ft_substr`

*   **Bytes Lost:** 8 bytes in 2 blocks
*   **Call Stack:**
    ```
    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
    by 0x10F6C1: ft_substr (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10DF6C: read_words (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10E228: process_next_token (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10E2D6: tokenize_line (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10974E: handle_parsing (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10985B: process_line (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10957D: input_loop (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x1095FC: main (in /home/batuhankiskac/Desktop/Minishell/minishell)
    ```
*   **Root Cause & Reason:** This leak is consistent with the previous report. The `read_words` function allocates memory using `ft_substr` for the `word` variable. This `word` is then passed to `token_add`, which creates a duplicate using `ft_strndup`. The original `word` allocated by `ft_substr` is not freed after its content is duplicated, leading to a memory leak for each token created.

## Still Reachable Memory (External Libraries)

As in previous reports, the majority of "still reachable" memory (232,888 bytes in 237 blocks) is associated with external libraries (`libreadline.so` and `libtinfo.so`). This is expected behavior for these libraries, which manage their own memory for features like command history and terminal capabilities throughout the program's lifetime. These do not indicate issues in your minishell's code.

## Conclusion

This report confirms the memory leak related to `ft_substr` in the tokenization process during heredoc handling. The other leaks observed in the `ls | echo deneme` test (related to `ft_strjoin` and `new_cmd`) were not present in this specific heredoc scenario, suggesting they might be triggered by different code paths or command structures. Addressing the `ft_substr` leak is crucial for improving memory management.

