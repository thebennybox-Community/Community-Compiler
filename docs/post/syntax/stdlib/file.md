# Files

File objects represent files or directories on the filesystem. They support reading and writing.

Defining is simple:

```dusk
var x: File("path/to/directory/or/file.txt");
```

Member functions:

`directoryContents(): str[]`: Returns list of files and subDirectories contained in this directory. If the object calling the function is a file instead, this returns `null`.

`exists(): bool`: Returns true if the file or directory exists, false if it doesn't.

`isDirectory(): bool`: Returns true if the file is a directory, false otherwise.

`isFile(): bool`: Returns opposite of `isDirectory()`, i.e., returns true for files and false for directories.
