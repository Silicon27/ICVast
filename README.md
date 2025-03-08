# Interpreted CVAST++

Interpreted implementation of the CVAST++ compiler.

## Installation

Ironically the C stands for "Compiled", so I'm not certain as to how I came to this.

Due to my persistence in, well, being lazy, I will in the follow section provide a command for you to self compile the code (instead of providing them myself :>).

> [!WARNING] 
> Prequisites required for compiling the code:
> - g++ (optimally above g++-11) or any other C++ compiler that supports C++20
> - git (for cloning the repository)

```bash
git clone https://github.com/Silicon27/ICVast.git
cd ICVast
chmod +x install.sh
```

Running the following command will automate the whole compilation process.
```bash
./install.sh
```

> [!NOTE]
> **In the case `./install.sh` does not work**, you can attempt to manually compile the code and add the stdlib via:
> ```bash
> g++ main.cpp -o InterpretedCVast -std=c++20
> ```
> Adding the stdlib (**Bash**):
> ```bash
> echo 'export CVAST_STDLIB="$(pwd)/stdlib"' >> ~/.bashrc && source ~/.bashrc
> ```
>for  **Zsh**:
> ```bash
> echo 'export CVAST_STDLIB="$(pwd)/stdlib"' >> ~/.zshrc && source ~/.zshrc
> ```

## Usage

The usage of the program is quite simple. You can run the program by providing the path to the file you want to interpret as an argument.

```bash
./InterpretedCVast path/to/file.cv
```

Checking the version of the program is also quite simple. You can do so by providing the `-v` flag.

```bash
./InterpretedCVast -v
```

If your unsure of the commands, you can always check the help menu by providing the `-h` flag.

```bash
./InterpretedCVast -h
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
