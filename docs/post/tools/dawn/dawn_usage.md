# The Dusk Programming Language

## [Post-Bootstrap](../../README.md) -> [Tools](../README.md) -> [Dawn](README.md) -> Dawn Usage

    dawn [-v] [-r] [-l <libraryname> ...] [-L <librarypath> ...] [<srcdir>] [-p] [-f <srcfile> ...] [-b <builddir>] [-o <outputfile>]

## Options

`-v | --verbose`: Verbose logging

`-r | --rebuild`: Force rebuild of all files.

`-l | --library`: Link library by name. Usually will resolve without explicitly defining using externs (see `dawn-auto-resolve.md`), but useful for compatibility with older versions that don't recognize certain externs.

`-L | --library-path`: Link library by file path. Only recommended when `-l` does not work.

`<srcdir>`: Main directory for source files, defaults to `.`

`-p | --pipe`: Get input file from stdin. Forces rebuild of file, even if stdin input hasn't changed.

`-f | --file`: Specify file from outside source directory.

`-b | --build-dir`: Specify directory to put temporary files, defaults to implementation-defined location. In Linux standard `dawn`, defaults to `~/.dawn/build/<srcdir-full-path>`

`-o | --output-file`: Specify final output file, defaults to `game` or, on Windows, `game.exe`
