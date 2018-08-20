# Community-Compiler

This is the Community Compiler project for The Benny Box Community.

Our main venue of discussion is the #community-compiler channel of The Benny Box public Discord server. Feel free to join with [this invite](https://discord.gg/X9ammDy) if you wish to contribute or discuss the project.

## Summary

The Community Compiler project primarily aims to create a new language called Dusk and a compiler for this language.

The language will include a standard library and various tools including Dawn, a package manager and build system for Dusk.

The full language specification can be found in [docs/post/syntax](./docs/post/syntax/).

## Current status

The Community Compiler is a work in progress.

We are currently in the process of finalizing the language specification and implementing pre-bootstrap Dusk compilers in C++ and Kotlin.

## Building from source

#### On Unix

Have the following dependencies installed:
- [Cmake](https://cmake.org/)
- [Git](https://git-scm.com/)

1. Clone the repository:

```sh
git clone https://github.com/thebennybox-Community/Community-Compiler.git
```

2. Change the working directory to [Community-Compiler/bootstrap/frontend](./bootstrap/frontend/):

```sh
cd Community-Compiler/bootstrap/frontend/
```

3. Run cmake:

```sh
cmake src
```

#### On Windows

Have the following dependencies installed:
- [Cmake](https://cmake.org/)
- [Git](https://git-scm.com/)

- Install Cmake
- Clone repository
- Run Windows-Gen-Project.bat
- Open `/build/compiler.sln`
