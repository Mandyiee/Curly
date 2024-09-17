# Curly

## Overview

This is a simple **Snake Game** implemented in C, designed to run on both Windows and Unix-based systems. The game features a dynamic console interface that adjusts to your terminal size, allowing you to control a snake, navigate the board, eat food, and grow the snake as you play. The game ends when the snake collides with the boundary or itself.

## Features

- **Cross-platform compatibility**: Runs on both Windows and Unix-based systems with different implementations for retrieving screen dimensions and capturing keyboard inputs.
- **Dynamic board resizing**: The game automatically adjusts to fit the size of your terminal window.
- **Snake movement**: Use the `W`, `A`, `S`, and `D` keys to control the snake's direction.
- **Food generation**: Randomly spawns food on the board for the snake to eat and grow longer.
- **Score tracking**: The game keeps track of how many food items the snake consumes.
- **Game over detection**: The game ends if the snake collides with the wall or itself.

## Controls

- `W` - Move Up
- `A` - Move Left
- `S` - Move Down
- `D` - Move Right
- `L` - Exit Game


## Setup Instructions

1. **Compile the Program**:
   - Use a C compiler like `gcc` to compile the game. Depending on your operating system, you can run the following commands:

   **For Unix-based systems (Linux/macOS):**
   ```bash
   gcc ga.c -o game
   ./game
   ```

   **For Windows (MinGW):**
   ```bash
   gcc ga.c -o game
   ./game
   ```

2. **Start the Game**:
   - After compilation, run the executable to start the game.
     ![Curly](https://raw.githubusercontent.com/Mandyiee/Curly/main/curly.png)
   

