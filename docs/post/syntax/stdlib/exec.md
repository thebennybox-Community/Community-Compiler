# The Dusk Programming Language

## [Post-Bootstrap](../../README.md) -> [Syntax](../README.md) -> [Standard Library](README.md) -> Execution

Fully static set of functions to handle execution of other programs.

Static functions:

`exec_sync(name: str, num_args: u32, ...): str`: Runs specified command syncronously and returns what was put into stdout.
