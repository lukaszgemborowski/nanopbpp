# nanopb++
Lightweight C++ wrapper(s) library for nanopb implementation.
The goal is to simplify some common tasks using C++ and metaprogramming
wherever possible.

## Current status
"work in progress" with very limited functionality and a small set of
unit tests. To see use-cases please see tests directory.

## building
as for now the library contains only C++ headers and it does not require anything special, only compiler with C++11 support. If you want to run tests than you need some additional things:

* cmake >= 3.2
* protoc
* gtest library development files

additionally you need to checkout nanopb submodule in the project (`git submodule update --init nanopb`) and run `make` in nanopb/generator/proto directory. After that you can create some temporary build directory, run `cmake` and then `make`.
