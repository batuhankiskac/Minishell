# Minishell

Minishell is a simple Unix shell clone implemented in C, designed as a project for educational purposes. It provides an interactive command-line environment where users can execute built-in and external commands, manage environment variables, handle input/output redirections, command pipelines, and more—all using standard POSIX system calls.

---

## Features

- **Interactive Command Prompt:**  
  Continuously prompts the user for input, supporting multi-line editing and command history through GNU Readline.

- **Built-in Commands:**  
  Supports several built-in shell commands:
  - `cd` — Change directory
  - `echo` — Print arguments to the terminal
  - `env` — List environment variables
  - `export` — Set environment variables
  - `pwd` — Print working directory
  - `unset` — Remove environment variables
  - `exit` — Exit the shell

- **External Command Execution:**  
  Finds and executes binaries in the system’s `$PATH`, with argument parsing and environment support.

- **Pipelines and Redirection:**  
  - Supports chaining commands using `|` (pipes).
  - Handles input `<`, output `>`, append `>>`, and heredoc `<<` redirections.

- **Variable Expansion:**  
  Expands environment variables and special shell variables within arguments and redirections, mimicking standard shell expansion behavior.

- **Signal Handling:**  
  Gracefully handles signals like `SIGINT` (Ctrl+C) and `SIGQUIT` (Ctrl+\), ensuring an interactive and robust user experience.

---

## Getting Started

### Prerequisites

- GCC or Clang compiler (supports C99 or later)
- GNU Readline library
- POSIX-compliant operating system (Linux, macOS, etc.)

### Build Instructions

1. **Clone the Repository:**
   ```sh
   git clone https://github.com/batuhankiskac/Minishell.git
   cd Minishell
   ```

2. **Build Minishell:**
   ```sh
   make
   ```

   This will compile the project and output the `minishell` binary.

---

## Usage

Start the shell by running:
```sh
./minishell
```

You will see a `minishell>` prompt. Enter any supported command, e.g.:
```sh
minishell> echo Hello, World!
Hello, World!
minishell> export NAME=Batuhan
minishell> echo $NAME
Batuhan
minishell> ls -l | grep minishell > result.txt
```

- Use `Ctrl+D` to exit the shell.
- Use `exit` command to quit.

---

## Project Structure

- `src/` — Source code organized by functional modules (main loop, built-ins, parser, executor, etc.)
- `include/` — Header files and core structure definitions
- `libft/` — Custom standard library implementation (as per 42 school convention)
- `Makefile` — Build instructions

---

## Contribution

Contributions are welcome! If you want to improve Minishell:

1. Fork the repository.
2. Create your feature branch (`git checkout -b feature/YourFeature`).
3. Commit your changes.
4. Push to the branch (`git push origin feature/YourFeature`).
5. Open a Pull Request describing your changes.

---

## License

This project is for educational purposes and does not carry a specific license. If you wish to use the code, please contact the author or open an issue.
