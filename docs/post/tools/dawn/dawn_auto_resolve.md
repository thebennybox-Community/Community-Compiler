# The Dusk Programming Language

## [Post-Bootstrap](../../README.md) -> [Tools](../README.md) -> [Dawn](README.md) -> Dawn Auto-Resolve

Sometimes, when you are making a project, you need to use `extern` libraries for things such as OpenGL/Vulkan, OpenAL/PulseAudio, and maybe also input utilities, but you don't know what library they are in, so you just slap the following into your program:

```dusk
extern {
    glInit();
    // ...
}

fn main() {
    glInit();
    // ...
}
```

If you don't remember that you're missing a library, and you call `duskc`, you will certainly get a linker error saying that it can't find your extern. This is where `dawn` comes in again.

    dawn

This command, when run in a directory with this file, doesn't actually throw any errors, since `dawn` automatically detects the extern and resolves the library used, which happens to resolve into `-lGL`, and the library gets linked in.

This doesn't always work, however. For libraries that `dawn` does not recognize, a message gets sent to us to find the required library and add it into `dawn`'s auto-resolver. Meanwhile, an error gets printed, and until the library is added, you will have to add it yourself.

This clearly isn't a foolproof system, but it is something to make the lives of Dusk programmers easier.
