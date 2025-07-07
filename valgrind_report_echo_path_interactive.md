# Valgrind Leak Report: `echo $PATH` (Interactive Mode)

**Date:** July 7, 2025
**Command Executed:** `echo "echo $PATH" | valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes ./minishell`

## Summary

The Valgrind analysis for the command `echo $PATH` executed in interactive mode shows:

*   **Definitely lost:** 314 bytes in 2 blocks
*   **Indirectly lost:** 0 bytes in 0 blocks
*   **Possibly lost:** 0 bytes in 0 blocks
*   **Still reachable:** 233,440 bytes in 237 blocks

## Analysis of Leaks Originating from Minishell Code

### Leak 1: Definitely Lost Bytes from `ft_substr`

*   **Bytes Lost:** 314 bytes in 2 blocks
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
*   **Root Cause & Reason:** This leak is consistent with previous reports. The `read_words` function uses `ft_substr` to extract a word (in this case, the expanded `$PATH` value) from the input line. This `word` is then passed to `token_add`. Inside `token_add`, `ft_strndup` is used to create a duplicate of this `word` for the `t_token` structure. However, the original `word` allocated by `ft_substr` in `read_words` is not freed after its content is duplicated. This leads to a memory leak for each successfully created token string. The larger size of this leak is due to the length of the `$PATH` environment variable.

## Still Reachable Memory (External Libraries)

As in previous reports, the majority of "still reachable" memory (233,440 bytes in 237 blocks) is associated with external libraries (`libreadline.so` and `libtinfo.so`). This is expected behavior for these libraries, which manage their own memory for features like command history and terminal capabilities throughout the program's lifetime. These do not indicate issues in your minishell's code.

## Conclusion

This report further confirms the persistent memory leak related to `ft_substr` in the tokenization process. This leak appears to be a fundamental issue in how tokens are managed after being extracted from the input line. Addressing this leak is crucial for improving memory management.
