#Movement Recorder for CS2

A tool for recording and playing back player movement in Counter-Strike 2. Implemented as a DLL using hooks and a frame-by-frame system.

## Features

* Record player movement (position, input, timings)
* Playback of recorded sequences
* Frame-by-frame data processing
* Separation of logic into recording, playback, and hooks

## Project Structure

```id="d1kq9f"
Movement-Recorder-for-CS2/
│
├── include/
│ ├── common.h
│ ├── frame.h
│ ├── hooks.h
│ ├── playback.h
│ └── recorder.h
│
├── src/
│ ├── dllmain.cpp
│ ├── frame.cpp
│ ├── hooks.cpp
│ ├── playback.cpp
│ └── recorder.cpp
```

## Architecture

### Recorder

Collects player state data:

* position
* user input
* timestamps

Saves them in a frame sequence (`Frame`).

### Playback

Plays back recorded data:

* reads a frame sequence
* applies it based on timings

### Hooks

Intercepts game functions and implements recording and playback logic.

## Build

The project does not contain a ready-made solution file (.sln). Build is performed manually.

Requirements:

* Compiler with C++17 support (MSVC / clang / gcc)
* Windows SDK

Result: A DLL file ready to be loaded into the game process.

## Usage

1. Inject the DLL into the Counter-Strike 2 process
2. Recording and playback are controlled through the project code (or an added interface)

## Note

This project uses offsets, signatures, and internal game structures, which depend on the client version. After Counter-Strike 2 updates, the following may need to be updated:

* offsets
* signatures
* structure descriptions and paddings

Correct operation is not guaranteed without updating this data.

## Limitations

Using such tools may violate the rules of the game. This project is provided for educational purposes. The user is responsible for its use.

## MIT License
