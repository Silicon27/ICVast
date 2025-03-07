# Interpreted CVAST++

Interpreted implementation of the CVAST++ compiler.

## Installation

Ironically the C stands for "Compiled", so I'm not certain as to how I came to this.

Due to my persistence in, well, being lazy, I will in the follow section provide a command for you to self compile the code (instead of providing them myself :>).

> [!WARNING]\
> Prequisites required for compiling the code:
> - g++ (optimally above g++-11) or any other C++ compiler that supports C++20
> - git (for cloning the repository)

```bash
cd ~/
git clone https://github.com/Silicon27/ICVast.git
cd ICVast

# post-download operations
chmod +x install.sh
./install.sh
g++ main.cpp -o InterpretedCVast -std=c++20
```

## Usage

The usage of the program is quite simple. You can run the program by providing the path to the file you want to interpret as an argument.

```bash
./InterpretedCVast path/to/file.cv
```

## Basic Syntax

CVast has a Rust-like syntax, with a few differences. Here is a basic example of a CVast program:

```
merge "../moduletest.cv" as test;
fn fortnite(that: int = 234, that: int) -> int {
    return this + that;
}
var x: int = 12322;
```

- `merge` is used to "*merge*" another module's symbol table to the current module.
- `fn` is used to define a function.
- `var` is used to define a variable.

`HelloWorld.cv`:

```
var str: string = "Hello, World!";
extern "writescr" (str);
```
