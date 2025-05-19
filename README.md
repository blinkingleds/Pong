# Pong Game

A simple implementation of the classic Pong game using SDL3.

## Game Components

- Main menu with start and quit options
- Basic paddle and ball physics
- Computer-controlled opponent
- Sound effects for collisions and scoring
- Score display

## Controls

- **Player Paddle (Right Side)**:
  - Up Arrow: Move Up
  - Down Arrow: Move Down

- **General**:
  - ESC: Exit Game

Note: The left paddle is computer-controlled.

## Requirements

- C++ Compiler (with C++11 support)
- CMake (version 3.10 or higher)
- SDL3, SDL3_ttf, SDL3_image development libraries

## Building with CMake (Recommended)

1.  **Prerequisites**: Ensure CMake, a C++11 compatible compiler, and the SDL3 development libraries (SDL3, SDL3_ttf, SDL3_image) are installed and accessible to CMake.

2.  **Configure & Generate Build Files**:
    Create a build directory (e.g., `build`) and run CMake from within it:
    ```bash
    mkdir build
    cd build
    cmake .. 
    ```
    *   You can specify a generator, for example, for Ninja: `cmake .. -G "Ninja"`
    *   If CMake has trouble finding SDL3, ensure the libraries are in your system's standard search paths, or set `SDL3_DIR`, `SDL3_ttf_DIR`, `SDL3_image_DIR` environment variables.

3.  **Compile**:
    From the build directory:
    ```bash
    # If using Ninja
    ninja

    # Or, for any generator (including Makefiles, Ninja, etc.)
    cmake --build .
    ```

4.  **Run**:
    The executable `PongGame` (or `PongGame.exe` on Windows) will be in the `build/out/` directory.
    ```bash
    # From the build directory
    ./out/PongGame 
    ```
    *   Ensure necessary runtime DLLs (SDL3, MinGW libraries on Windows) are accessible, either by being in the same directory as the executable or in the system's PATH. The CMake setup attempts to copy SDL DLLs from a relative `lib` folder if present.

### Cleaning the Build

*   **Standard Clean**: Most build systems provide a clean target.
    ```bash
    # If using Ninja, from the build directory:
    ninja clean
    
    # Or, for any generator:
    cmake --build . --target clean
    ```
*   **Full Clean**: For a complete refresh, delete the entire `build` directory and start the configuration process again.

## Building Manually (Alternative)

1. Requirements:
   - G++
   - SDL3, SDL3_ttf, SDL3_image
2. Setup:
   - Put SDL3 headers in `include` directory
   - Put SDL3 libraries in `lib` directory
3. Build Command:
   ```bash
   g++ -g src/Pong_Game.cpp src/gamerendering.cpp src/game.cpp src/mainmenustate.cpp src/gameplaystate.cpp -o out/Pong.exe -I./include -L./lib ./lib/SDL3.lib ./lib/SDL3_ttf.lib ./lib/libSDL3_image.dll.a
   ```
4. Copy DLLs: Copy SDL3.dll, SDL3_ttf.dll, and SDL3_image.dll to the `out/` directory.
