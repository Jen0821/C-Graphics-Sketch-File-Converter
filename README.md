# C Implementation of a Custom Graphics Sketch File Converter

## Overview

This project is a low-level implementation in **C** to read, parse, process, and convert files written in a custom **Graphics Sketch File Format**. The work demonstrates proficiency in binary file I/O, strict data structure implementation, and manipulating graphic primitives. The primary task is to correctly parse the structure of the file (including basic, intermediate, and advanced versions) and generate a new, processed output file or "patch."

## Technical Focus

The core challenge involves correctly parsing and manipulating various data blocks defined by the custom file specification.

### Custom Data Blocks

The program implements handlers for the following graphic and control data blocks:

* `COLOUR`: Handles colour definitions and changes.
* `TARGETX`, `TARGETY`: Manages coordinate data for drawing operations.
* `DXY`: Data related to dynamic movement or relative coordinate changes.
* **`FLOCK`**: A complex, proprietary data block that requires specialised parsing logic.

### File Format Handling

* **Version Support**: The code is structured to correctly read and interpret data across the Basic, Intermediate, and Advanced versions of the Sketch File Format.
* **Binary I/O**: Direct manipulation of file streams and binary data structures is necessary for accurate data extraction and reconstitution into the output patch file.

## Compilation and Usage

The project uses the standard C development environment and compilation process.

### Development Environment

* GCC compiler (or Clang)
* External libraries for environment setup (used via `pkg-config`).

### Compilation Example

The original coursework specified using robust compilation flags for safety and testing:

```bash
# Example compilation command using provided safety and testing flags
clang -DTESTING -Wall -pedantic -g -fsanitize=undefined pkg-config --libs --cflags add...
