# Valgrind Leak Report: `ls | echo deneme` (Interactive Mode)

**Date:** July 7, 2025
**Command Executed:** `echo "ls | echo deneme" | valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes ./minishell`

## Summary

The Valgrind analysis for the command `ls | echo deneme` executed in interactive mode shows:

*   **Definitely lost:** 55 bytes in 4 blocks
*   **Indirectly lost:** 19 bytes in 2 blocks
*   **Possibly lost:** 0 bytes in 0 blocks
*   **Still reachable:** 235,930 bytes in 492 blocks

## Analysis of Leaks Originating from Minishell Code

### Leak 1: Indirectly Lost Bytes from `ft_strjoin`

*   **Bytes Lost:** 3 bytes in 1 block
*   **Call Stack:**
    ```
    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
    by 0x10F7A6: ft_strjoin (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10D584: append_literal (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10D79F: expand_text_chunk (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10D6C4: expand_string (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10DA1F: expand_on_args (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10D148: expander (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10978F: handle_parsing (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10985B: process_line (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10957D: input_loop (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x1095FC: main (in /home/batuhankiskac/Desktop/Minishell/minishell)
    ```
*   **Root Cause & Reason:** This leak occurs within the string expansion process. It appears that an intermediate string created by `ft_strjoin` (likely within `append_literal` or `expand_text_chunk` during variable or tilde expansion) is not being properly freed. When `ft_strjoin` is used to build up a new string, the old string (if it was dynamically allocated) needs to be freed after the new, combined string is created. This suggests a missing `free()` call for an intermediate string in the expansion logic.

### Leak 2: Definitely Lost Bytes from `ft_substr`

*   **Bytes Lost:** 15 bytes in 3 blocks
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
*   **Root Cause & Reason:** The `read_words` function uses `ft_substr` to extract a word from the input line. This `word` is then passed to `token_add`. Inside `token_add`, `ft_strndup` is used to create a duplicate of this `word` for the `t_token` structure. However, the original `word` allocated by `ft_substr` in `read_words` is not freed after `token_add` successfully duplicates it. It is only freed if `token_add` fails. This leads to a memory leak for each successfully created token string.

### Leak 3: Definitely Lost Bytes from `new_cmd`

*   **Bytes Lost:** 59 bytes (40 direct, 19 indirect) in 1 block
*   **Call Stack:**
    ```
    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
    by 0x10E448: new_cmd (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10E565: build_command_list (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10975F: handle_parsing (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10985B: process_line (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x10957D: input_loop (in /home/batuhankiskac/Desktop/Minishell/minishell)
    by 0x1095FC: main (in /home/batuhankiskac/Desktop/Minishell/minishell)
    ```
*   **Root Cause & Reason:** This leak indicates that the `t_command` structure, which represents a parsed command, is allocated by `new_cmd` but is not being freed after the command has been executed or processed. The `process_line` function is responsible for handling the entire command lifecycle, and it should include a cleanup mechanism (e.g., `clear_command_list`) to free the `t_command` structure and its associated data (like arguments and redirections) once they are no longer needed.

## Still Reachable Memory (External Libraries)

As in previous reports, the majority of "still reachable" memory (235,930 bytes in 492 blocks) is associated with external libraries (`libreadline.so` and `libtinfo.so`). This is expected behavior for these libraries, which manage their own memory for features like command history and terminal capabilities throughout the program's lifetime. These do not indicate issues in your minishell's code.

## Conclusion

This report identifies three distinct memory leaks within your minishell's custom code related to string handling during tokenization and expansion, and the lifecycle management of command structures. Addressing these leaks will significantly improve the memory hygiene of your minishell.
