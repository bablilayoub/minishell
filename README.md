# Minishell - More like : "Minihell"

## Description
The objective of this project is to create a simple shell. The shell should be able to prompt the user for a command, locate the executable, and execute the command. The shell should also be able to handle errors and built-in commands.

## Features
- Prompt the user for a command
- Locate the executable
- Execute the command
- Handle errors
- Built-in commands
- Environment variables
- Signal handling
- Pipes
- Redirections
- Line editing
- History
- and more...

## Built-in commands
- `cd`
- `echo`
- `env`
- `exit`
- `export`
- `pwd`
- `unset`


## Usage
```bash
$ make
$ ./minishell
```

# Journey to the Minihell
## Parsing (My part)
The first step was to parse the input. We had to split the input into tokens. We used a simple lexer to do this. The lexer is a simple state machine that reads the input character by character and determines the type of token. The lexer can handle multiple types that you can find in the `parser.h` file.

The after the lexer, we had to parse the tokens. We used an advanced lexer to do this. The parser is a simple state machine that reads the tokens and determines the type of command. 

This was the most challenging part of the project. We had to handle multiple types of commands, such as:
- Simple commands
- Pipes
- Redirections
- Environment variables
- and more...

The project was a success. but with a lot of bugs. We had to fix a lot of bugs and edge cases. We also had to refactor the code to make it more readable and maintainable.
When they tell you that the parsing is the most challenging part of the project, they are not lying.
It was a long and hard journey, but we made it. We are proud of what we have accomplished.

## Execution (Achraf's part)
The second step was to execute the command. We had to locate the executable and execute the command. We used the `execve` system call to execute the command. We also had to handle errors and built-in commands.

Duping, forking, and waiting for the child process was the easy part. The hard part was to handle the built-in commands.
Trust me, handling the built-in commands is not as easy (you will see why when you try to implement it).

(We will not talk about the built-in commands, because it is a secret, and we don't want to spoil the fun for you)

## Authors
- [**Ayoub Baba**](https://github.com/bablilayoub)
- [**Achraf Laalalma**](https://github.com/Lfrida)

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
