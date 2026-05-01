# Bin Runtime Environment Design

**Goal:** Create a dedicated ignored `bin/` runtime directory that can hold runnable `Debug` and `Release` builds, including copied Qt runtime dependencies, without relying on the active shell environment.

**Context:** The repository currently builds into `build/`, and the existing documentation suggests generic `cmake ..` usage. On this machine that can select an incompatible toolchain. The runtime output also stays mixed into the CMake build tree instead of a dedicated runnable directory.

## Recommended Approach

Use fixed Windows batch scripts plus `windeployqt` to produce self-contained runtime folders under `bin/Debug` and `bin/Release`.

This keeps the solution small and practical:
- `build/` remains the CMake/MSVC build tree
- `bin/<Config>/` becomes the runnable output tree
- deployment is handled by Qt's supported Windows deployment tool instead of a fragile manual DLL copy list

## Directory Layout

- `build/`
  - CMake and Visual Studio build artifacts
- `bin/Debug/`
  - `QFileMatrixApp.exe`
  - Qt runtime DLLs
  - platform/plugin subdirectories produced by `windeployqt`
- `bin/Release/`
  - same structure for release output

`bin/` should be gitignored because it is generated, machine-specific, and can become large.

## Script Design

### `configure_app.bat`

Responsibilities:
- load Visual Studio 2022 x64 environment through `vcvars64.bat`
- create `build/` if missing
- configure CMake with `Visual Studio 17 2022`, `x64`, and the Qt `msvc2019_64` prefix path

This removes ambiguity from the generator/toolchain selection.

### `build_app.bat [Debug|Release]`

Responsibilities:
- default to `Debug` when no argument is given
- ensure the build tree is configured
- build `QFileMatrixApp` for the requested configuration

### `deploy_app.bat [Debug|Release]`

Responsibilities:
- default to `Debug`
- ensure the requested configuration has been built
- create `bin/<Config>/`
- copy `build/<Config>/QFileMatrixApp.exe` into `bin/<Config>/`
- run `windeployqt` against the copied executable so the runtime folder contains required Qt dependencies

### `run_app.bat [Debug|Release]`

Responsibilities:
- default to `Debug`
- call deploy so `bin/<Config>/` is fresh enough to run
- launch `bin/<Config>/QFileMatrixApp.exe`

## Documentation Changes

Update `README.md` so local usage flows through the scripts instead of generic `cmake ..` commands.

The documentation should clearly separate:
- `build/` as the compiler output tree
- `bin/` as the runnable packaged environment

## Ignore Rules

Update `.gitignore` to include:
- `bin/`

While touching the file, normalize it back to plain text because the current file contains corrupted trailing bytes.

## Verification

Verification should prove both the build and the runtime packaging workflow:
- run the configure/build flow for `Debug`
- run the deploy flow for `Debug`
- verify `bin/Debug/QFileMatrixApp.exe` exists
- repeat deploy for `Release` if supported by the current build tree
- confirm `.gitignore` excludes `bin/`

## Risks

- `windeployqt` output can vary slightly by Qt installation, so the scripts should target the known Qt path already used by this repository.
- Debug deployment depends on the debug Qt runtime being installed locally, which is acceptable for this development machine.
