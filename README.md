# Conway's Game of Life in C++ with SDL2

A graphical implementation of John Conway's famous cellular automaton, "The Game of Life," built from scratch in C++ using the SDL2 multimedia library.

[![Language](https://img.shields.io/badge/Language-C%2B%2B-blue.svg)](https://isocpp.org/)
[![Library](https://img.shields.io/badge/Library-SDL2-brightgreen.svg)](https://www.libsdl.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

![Game of Life Demo](https://i.imgur.com/YOUR_DEMO_GIF_URL.gif)
*(**Note**: It is highly recommended to replace the link above with a GIF of your application in action. You can use a tool like [ScreenToGif](https://www.screentogif.com/) or [GIPHY Capture](https://giphy.com/apps/giphycapture) to create one.)*

---

## 📜 Project Overview

This project is a visual and interactive simulation of Conway's Game of Life on a fixed 20x20 grid. Users can create initial patterns by clicking on cells, control the simulation's flow, adjust its speed, and even save and load specific generations to and from a text file. It leverages the **SDL2**, **SDL_image**, and **SDL_ttf** libraries to handle graphics, windowing, and text rendering.

---

## ✨ Features

*   **Interactive Grid**: A 20x20 grid where you can toggle the state of any cell (alive or dead) with a simple mouse click.
*   **Full Simulation Control**:
    *   **Start**: Begins the step-by-step evolution of the cells.
    *   **Stop**: Pauses the simulation at its current state.
    *   **Clear**: Resets the entire grid, setting all cells to dead.
*   **Adjustable Speed**: Use the **Up** and **Down** arrow keys to increase or decrease the delay between generations, effectively speeding up or slowing down the simulation.
*   **Generation Counter**: A counter on the screen displays the current generation number.
*   **Persistent State Saving**: When you launch the application, you provide a file name. Each new generation's state is automatically appended to a corresponding `.txt` file.
*   **Load Specific Generations**: Use the **Upload** feature to load a specific generation from the history file back onto the grid for further simulation or analysis.

---

## 📖 How It Works: The Rules of Life

The simulation evolves based on a few simple rules applied to each cell at every step:
1.  A **live** cell with **2 or 3** live neighbors **survives** to the next generation.
2.  A **dead** cell with **exactly 3** live neighbors **becomes a live cell** (is "born").
3.  All other live cells **die** from underpopulation (<2) or overpopulation (>3), and all other dead cells **remain dead**.

---

## 🛠️ Technology Stack

*   **Language**: C++
*   **Graphics/Windowing**: [SDL2](https://www.libsdl.org/)
*   **Image Loading**: [SDL_image](https://www.libsdl.org/projects/SDL_image/) (for UI elements like buttons)
*   **Font Rendering**: [SDL_ttf](https://www.libsdl.org/projects/SDL_ttf/) (for the generation counter and other text)

---

## 🚀 Setup and Compilation

To compile and run this project, you will need a C++ compiler and the SDL2 development libraries.

#### 1. Prerequisites
*   A C++ compiler (like `g++`).
*   SDL2 development libraries.
*   SDL2_image development libraries.
*   SDL2_ttf development libraries.

#### 2. Install Dependencies

*   **On Debian/Ubuntu:**
    ```bash
    sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
    ```
*   **On macOS (using Homebrew):**
    ```bash
    brew install sdl2 sdl2_image sdl2_ttf
    ```
*   **On Windows:**
    Follow the excellent tutorials on [Lazy Foo' Productions](https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php) to set up SDL2 with your preferred IDE (e.g., Visual Studio or Code::Blocks).

#### 3. Compile the Source Code
Navigate to the project's source directory in your terminal and compile the main file:
```bash
g++ -o GameOfLife game.cpp -lSDL2 -lSDL2_image -lSDL2_ttf
```

#### 4. Run the Application
```Bash
./GameOfLife
```
You will be prompted in the terminal to enter a name for your session file before the GUI window appears.

## 🖥️ Control & Usage

* Left Click	Toggles a cell between its alive and dead states.
* 'Start' Button	Begins running the simulation generation by generation.
* 'Stop' Button	Pauses the simulation at the current generation.
* 'Clear' Button	Clears the grid, killing all cells.
* 'Upload' Button	Prompts you to enter a generation number to load from the session file.
*  Up Arrow Key	Decreases the step delay (speeds up the simulation).
*  Down Arrow Key	Increases the step delay (slows down the simulation).
