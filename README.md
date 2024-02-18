# cpp-msgpack-light

[![GitLab Release](https://img.shields.io/gitlab/v/release/54580872?sort=semver)](https://gitlab.com/MusicScience37Projects/utility-libraries/cpp-msgpack-light/-/releases)
[![C++ standard](https://img.shields.io/badge/standard-C%2B%2B17-blue?logo=c%2B%2B)](https://en.cppreference.com/w/cpp/compiler_support/17)
[![pipeline status](https://gitlab.com/MusicScience37Projects/utility-libraries/cpp-msgpack-light/badges/main/pipeline.svg)](https://gitlab.com/MusicScience37Projects/utility-libraries/cpp-msgpack-light/-/commits/main)
[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit&logoColor=white)](https://github.com/pre-commit/pre-commit)

A light library to serialize [MessagePack](https://msgpack.org/).

This library implements
[MessagePack specification updated on 2021/4/29](https://github.com/msgpack/msgpack/blob/8aa09e2a6a9180a49fc62ecfefe149f063cc5e4b/spec.md)
(latest as of 2024/2/14).

## Documentation

- [Documentation built on main branch](https://cpp-msgpack-light-musicscience37projects-utility-a06a38f487cafc.gitlab.io/)

## Repositories

- Main in GitLab: [https://gitlab.com/MusicScience37Projects/utility-libraries/cpp-msgpack-light](https://gitlab.com/MusicScience37Projects/utility-libraries/cpp-msgpack-light)
- Mirror in GitHub: [https://github.com/MusicScience37/cpp-msgpack-light](https://github.com/MusicScience37/cpp-msgpack-light)

## Installation

### Simply Including Headers

This library is header-only,
so you can simply include headers
setting `include` directory to an include directory of compilers.
This library depends only on C++ standard library,
so no further installation is required.

### Via vcpkg

This library can be installed via vcpkg using following configurations:

- Add a vcpkg registry
  [`https://gitlab.com/MusicScience37Projects/vcpkg-registry`](https://gitlab.com/MusicScience37Projects/vcpkg-registry)
  in `vcpkg-configuration.json`.

  Example:

  ```json
  {
    "$schema": "https://raw.githubusercontent.com/microsoft/vcpkg-tool/main/docs/vcpkg-configuration.schema.json",
    "default-registry": {
      "kind": "git",
      "repository": "https://github.com/Microsoft/vcpkg",
      "baseline": "13bde2ff13192e1b2fdd37bd9b475c7665ae6ae5"
    },
    "registries": [
      {
        "kind": "git",
        "repository": "https://gitlab.com/MusicScience37Projects/vcpkg-registry",
        "baseline": "60b09d7b836e5a731d864a7227d68901ec8f3071",
        "packages": ["cpp-msgpack-light"]
      }
    ]
  }
  ```

- Add `cpp-msgpack-light` in `vcpkg.json`

  Example:

  ```json
  {
    "$schema": "https://raw.githubusercontent.com/microsoft/vcpkg-tool/main/docs/vcpkg.schema.json",
    "dependencies": ["cpp-msgpack-light"]
  }
  ```

## How to Build in the Repository

### Dependencies

A development environment including dependencies can be created using
[Devcontainer of VSCode](https://code.visualstudio.com/docs/remote/containers).

To build this repository,
you will require following dependencies:

- [Python](https://www.python.org/) 3.11
  - You may want to use [pyenv](https://github.com/pyenv/pyenv).
- [poetry](https://python-poetry.org/)
  - Required Python packages can be installed using poetry.
    Execute the command `poetry install` on this directory.
  - [pre-commit](https://pre-commit.com/)
    will be installed via poetry, and used for some checks.
    Execute `poetry run pre-commit install` on this directory
    if you develop this library.
- [CMake](https://cmake.org/)
- C++ 17 Compiler
  - Following compilers are tested in CI:
    - GCC 10
    - GCC 12
    - Clang 15
    - Clang 16
    - MSVC 19

Additionally, you will require following dependencies
to build documentation.

- [Doxygen](https://www.doxygen.nl/index.html)
- [Graphviz](https://graphviz.org/)
- [PlantUML](https://plantuml.com)
  - Set `PLANTUML_JAR_PATH` environment variable to specify the place of `plantuml.jar` file.
- Java runtime environment (JRE) for PlantUML.

### Build

If you created the development environment via Devcontainer of VSCode,
you can build this repository using CMake Tools extension.

To build using command line, follow the steps below.

1. Start a shell of poetry.

   ```bash
   cd <this-directory>
   poetry install
   poetry shell
   ```

2. Download vcpkg if you haven't already.

   ```bash
   git submodule update --init
   ```

3. Configure.

   ```bash
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
   ```

4. Optionally edit options.

   ```bash
   <some-editor> CMakeCache.txt
   ```

5. Build.

   ```bash
   cmake --build .
   ```

6. Optionally run tests.

   ```bash
   ctest
   ```

## License

This project is licensed under [Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0).
