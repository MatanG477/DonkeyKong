# Donkey Kong Console Game

## Overview

This is a console-based recreation of the classic Donkey Kong game, developed in C++. The project implements various game mechanics such as Mario movement, barrels, ghosts, ladders, and the ultimate g[...]

## How to Play

The objective of the game is to control Mario (@) and navigate through obstacles to rescue Pauline ($). Donkey Kong (&) throws barrels (O), and ghosts (x) roam the platforms. Use ladders (H) to climb [...]

## Features

- ASCII-based graphics with optional color mode.
- Dynamic game mechanics including Mario (@), Donkey Kong (&), barrels (O), ghosts (x), ladders (H), and Pauline ($).
- Multiple stages loaded from .screen files.
- Save and load functionality for recording and replaying games.
- Silent mode (-silent) for automated result verification.
- Organized file structure for better maintainability.
- CI/CD support (planned) for automated testing.

## Installation & Execution
1. Clone the repository

git clone https://github.com/adi0258/DonkeyKong.git
cd DonkeyKongConsoleGame

2. Open the project in Visual Studio

Open DonkeyKong1.sln in Visual Studio 2022.

Build the project in Release mode.

3. Run the game

Execute the compiled binary:

dkong.exe

Or with command-line options:

dkong.exe -load
dkong.exe -save
dkong.exe -silent

## Controls

LEFT → A / a

RIGHT → D / d

UP / JUMP → W / w

DOWN → X / x

STAY → S / s

Use Hammer → P / p

Pause → ESC

## Screenshots



## Contributing

Feel free to fork the repository and submit pull requests with improvements or bug fixes.

## License

This project is open-source. See LICENSE file for details.

