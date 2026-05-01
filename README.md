# QFileMatrix

A Qt-based desktop application for file matrix measurement and visualization.

## Build Requirements

- Qt 6.x
- CMake 3.21+
- Visual Studio 2022 with C++ tools

## Supported Local Workflow

Use the provided scripts from the repository root so the project always builds with MSVC and the Qt `msvc2019_64` runtime.

### Configure

```bat
configure_app.bat
```

### Build

```bat
build_app.bat Debug
build_app.bat Release
```

### Deploy Runnable Environment

```bat
deploy_app.bat Debug
deploy_app.bat Release
```

### Run

```bat
run_app.bat Debug
run_app.bat Release
```

## Output Directories

- `build/` - CMake and Visual Studio build artifacts
- `bin/` - Runnable packaged environment created by `windeployqt`

## Project Structure

- `src/` - Source code
- `tests/` - Unit tests
- `resources/` - Resource files
- `docs/` - Design and implementation notes
