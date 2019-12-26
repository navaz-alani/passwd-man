# `passwd-man`: A Command Line Password Manager

`pman` is a command line utility to manage credentials.
It is available for MacOS and is yet to be tested on Linux.

## Requirements

* `python3` and `pip3`. This is necessary to interact with
the system's clipboard.
* A `C` compiler; default is `clang`. To change this, set the
desired compiler in the `Makefile`.

## Building the Program

In order to build `passwd-man`, `cd` into the project folder
and run `make build`. This will produce an executable called
`passwd-man.

Adding the project directory to the system `$PATH` variable
will allow the user to call the program from any directory.
