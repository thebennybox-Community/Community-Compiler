# The Dusk Programming Language

## [Post-Bootstrap](../../README.md) -> [Syntax](../README.md) -> [Standard Library](README.md) -> Files

Fully static set of functions to handle files.

Static functions:

`combinePath(num_parts: u32, ...): str`: Returns combination of all path fragments. This also handles trailing `/` after each part.

`directoryContents(path: str): str[]`: Returns list of files and subDirectories contained in this directory. If the object calling the function is a file instead, this returns `null`.

`exists(path: str): bool`: Returns true if the file or directory exists, false if it doesn't.

`getAbsolutePath(path: str): str`: Takes relative path (such as `../assets/gameData.dat`) and converts it to an absolute path (`/home/joe/awesomeGame/assets/gameData.dat`).

`getName(path: str): str`: Returns name of file.

`getSubFiles(path: str): str[]`: Returns all files and directories in the specified directory. If the input isn't a directory, returns `null`.

`isDirectory(path: str): bool`: Returns true if the file is a directory, false otherwise.

`isFile(path: str): bool`: Returns opposite of `isDirectory()`, i.e., returns true for files and false for directories.

`readAllText(path: str): str`: Returns full text content of specified file (native newline encoding etc. converted to cross-platform compatible charset). If the input isn't a file, returns `null`.

`readAllBytes(path: str): u8[]`: Returns full byte content of specified file (native newline encoding etc. not converted). If the input isn't a file, returns `null`.
