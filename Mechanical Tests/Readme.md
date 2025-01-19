# Mechanical Tests Program

## Overview
This program is designed for conducting mechanical tests on metallic materials. It allows users to select the type of graph (true or engineering) and choose from a list of mechanical tests, including bending and tensile tests for steel and aluminum.

## Features
- User-friendly interface for selecting graph types and tests.
- Supports two types of graphs: true and engineering.
- Provides options for four different mechanical tests:
  1. Bending of steel
  2. Tensile test of steel
  3. Tensile test of aluminum
  4. Compression test of aluminum

## Requirements
- C compiler (e.g., GCC)
- Standard C library

## Usage
1. Compile the program using a C compiler:
   ```bash
   gcc -o mechanical_tests mechanical_tests.c
   ```
2. Run the compiled program:
   ```bash
   ./mechanical_tests
   ```
3. Follow the on-screen prompts to select the graph type and test number.

## Input
- The program prompts the user to enter the type of graph:
  - For true graph: `true`
  - For engineering graph: `engineering`
- The user is then prompted to enter the test number corresponding to the desired mechanical test.

## Output
- The program displays the results of the selected mechanical test based on the chosen graph type.

## Error Handling
- The program checks for valid input for both the graph type and test number. If invalid input is detected, an error message is displayed, and the program exits with a failure status.

## Author
- Αλέξανδρος Καρδούλιας

## License
This program is open-source and can be modified and distributed under the terms of the MIT License.
